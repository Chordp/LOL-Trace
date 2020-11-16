#include "Main.h"
#include "Inject.h"
#include "Process.h"
#include "SSDT.h"
#include "shell.h"
fn_NtAllocateVirtualMemory	pfn_NtAllocateVirtualMemory;
fn_NtReadVirtualMemory		pfn_NtReadVirtualMemory;
fn_NtWriteVirtualMemory		pfn_NtWriteVirtualMemory;
fn_NtProtectVirtualMemory	pfn_NtProtectVirtualMemory;
PUNICODE_STRING g_pRegistryPath = NULL;
PWORK_QUEUE_ITEM g_pUnloadWorkItem = NULL;
//
// 搜索字符串,来自blackbone
//
LONG SafeSearchString(IN PUNICODE_STRING source, IN PUNICODE_STRING target, IN BOOLEAN CaseInSensitive)
{
	ASSERT(source != NULL && target != NULL);
	if (source == NULL || target == NULL || source->Buffer == NULL || target->Buffer == NULL)
		return STATUS_INVALID_PARAMETER;

	// Size mismatch
	if (source->Length < target->Length)
		return -1;

	USHORT diff = source->Length - target->Length;
	for (USHORT i = 0; i <= (diff / sizeof(WCHAR)); i++)
	{
		if (RtlCompareUnicodeStrings(
			source->Buffer + i,
			target->Length / sizeof(WCHAR),
			target->Buffer,
			target->Length / sizeof(WCHAR),
			CaseInSensitive
		) == 0)
		{
			return i;
		}
	}

	return -1;
}

VOID INJECT_ROUTINE_X86(
	_In_ PVOID StartContext)
{

	PINJECT_PROCESSID_DATA	injectdata = (PINJECT_PROCESSID_DATA)StartContext;

	DPRINT("x86注入 pid=%d %p\n", injectdata->pid, injectdata->imagebase);


	//
	//1.attach进程，2.找导出表ZwContinue 3.组合shellcode 4.申请内存  5.Hook ZwContinue 
	//

	ULONG			trace = 1;

	PEPROCESS		process;
	NTSTATUS		status;
	KAPC_STATE		apc;
	BOOLEAN			attach = false;

	ULONG64			pfnZwContinue = 0;
	PVOID			pZwContinue;

	PVOID			alloc_ptr = NULL;
	SIZE_T			alloc_size = 0;
	SIZE_T			alloc_pagesize = 5;
	ULONG			alloc_oldProtect = 0;

	ULONG			dllPos, shellcodePos;

	INJECT_PROCESSID_PAYLOAD_X86	payload = { 0 };

	UCHAR	hookbuf[5];
	ULONG	dwTmpBuf;
	SIZE_T	returnLen;

	//KdBreakPoint();

	//
	//1.attach进程
	//
	status = PsLookupProcessByProcessId(injectdata->pid, &process);
	if (!NT_SUCCESS(status) && process == NULL)
	{
		goto __exit;
	}
	ObDereferenceObject(process);

	trace = 2;
	KeStackAttachProcess(process, &apc);
	attach = true;

	//
	//2.找导出表ZwContinue
	//
	pfnZwContinue = (ULONG64)Process::GetModuleExport((PVOID)injectdata->imagebase, "ZwContinue");
	DPRINT("pfnZwContinue %p\n", pfnZwContinue);
	if (pfnZwContinue == NULL)
	{
		goto __exit;
	}
	trace = 3;

	status = pfn_NtReadVirtualMemory(NtCurrentProcess(),
		(PVOID)pfnZwContinue,
		&payload.oldData,
		sizeof(payload.oldData),
		NULL);
	if (!NT_SUCCESS(status))
	{
		goto __exit;
	}

	trace = 4;


	//
	//3.计算shellcode 大小
	//
	alloc_size = sizeof(INJECT_PROCESSID_PAYLOAD_X86) + sizeof(MemLoadShellcode_x86) + sizeof(shell);

	payload.saveReg[0] = 0x60; //pushad
	payload.saveReg[1] = 0x9c; //pushfd

	payload.restoneHook[0] = 0xB9; // mov ecx,5
	payload.restoneHook[5] = 0xBE; // mov edi,xxxx
	payload.restoneHook[10] = 0xBF; // mov esi,xxxx
	payload.restoneHook[15] = 0xF3;
	payload.restoneHook[16] = 0xA4; // rep movsb

	payload.invokeMemLoad[0] = 0x68; // push xxxxxx
	payload.invokeMemLoad[5] = 0xE8; // call xxxxxx


	payload.eraseDll[0] = 0xB0;
	payload.eraseDll[2] = 0xB9;
	payload.eraseDll[7] = 0xBF;
	payload.eraseDll[12] = 0xF3;
	payload.eraseDll[13] = 0xAA;

	payload.restoneReg[0] = 0x9D; // popfd
	payload.restoneReg[1] = 0x61; // popad

	payload.jmpOld[0] = 0xE9;// jmp xxxxxx



	//
	//4.申请内存
	//
	status = pfn_NtAllocateVirtualMemory(NtCurrentProcess(),
		&alloc_ptr,
		NULL,
		&alloc_size,
		MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	if (!NT_SUCCESS(status))
	{
		goto __exit;
	}
	trace = 5;
	//
	//5. Hook ZwContinue 
	//

	//计算dll 和shellcode位置
	dllPos = PtrToUlong(alloc_ptr) + sizeof(INJECT_PROCESSID_PAYLOAD_X86) - 2;
	shellcodePos = dllPos +sizeof(shell);

	//恢复hook
	dwTmpBuf = sizeof(payload.oldData);
	memcpy(&payload.restoneHook[1], &dwTmpBuf, sizeof(ULONG));
	dwTmpBuf = PtrToUlong(alloc_ptr) + (sizeof(INJECT_PROCESSID_PAYLOAD_X86) - 7);
	memcpy(&payload.restoneHook[6], &dwTmpBuf, sizeof(ULONG));
	memcpy(&payload.restoneHook[11], &pfnZwContinue, sizeof(ULONG));

	//调用内存加载
	memcpy(&payload.invokeMemLoad[1], &dllPos, sizeof(ULONG));
	dwTmpBuf = shellcodePos - (PtrToUlong(alloc_ptr) + 24) - 5;
	memcpy(&payload.invokeMemLoad[6], &dwTmpBuf, sizeof(ULONG));


	//擦除DLL
	dwTmpBuf = sizeof(MemLoadShellcode_x86) + sizeof(shell);
	memcpy(&payload.eraseDll[3], &dwTmpBuf, sizeof(ULONG));
	memcpy(&payload.eraseDll[8], &dllPos, sizeof(ULONG));

	//跳回去
	dwTmpBuf = (ULONG)pfnZwContinue - (PtrToUlong(alloc_ptr) + (sizeof(INJECT_PROCESSID_PAYLOAD_X86) - 12)) - 5;
	memcpy(&payload.jmpOld[1], &dwTmpBuf, sizeof(ULONG));

	status = pfn_NtWriteVirtualMemory(NtCurrentProcess(),
		alloc_ptr,
		&payload,
		sizeof(payload),
		&returnLen);
	if (!NT_SUCCESS(status))
	{
		goto __exit;
	}
	trace = 6;


	status = pfn_NtWriteVirtualMemory(NtCurrentProcess(),
		(PVOID)dllPos,
		shell,
		sizeof(shell),
		&returnLen);
	if (!NT_SUCCESS(status))
	{
		goto __exit;
	}
	trace = 7;


	status = pfn_NtWriteVirtualMemory(NtCurrentProcess(),
		(PVOID)shellcodePos,
		&MemLoadShellcode_x86,
		sizeof(MemLoadShellcode_x86),
		&returnLen);
	if (!NT_SUCCESS(status))
	{
		goto __exit;
	}
	trace = 8;


	//
	//Hook
	//

	dwTmpBuf = PtrToUlong(alloc_ptr) - (ULONG)pfnZwContinue - 5;
	hookbuf[0] = 0xE9;
	memcpy(&hookbuf[1], &dwTmpBuf, sizeof(ULONG));


	//备份一遍原地址
	pZwContinue = (PVOID)pfnZwContinue;
	status = pfn_NtProtectVirtualMemory(NtCurrentProcess(),
		(PVOID*)&pfnZwContinue,
		&alloc_pagesize,
		PAGE_EXECUTE_READWRITE,
		&alloc_oldProtect);
	if (!NT_SUCCESS(status))
	{
		goto __exit;
	}
	trace = 9;

	status = pfn_NtWriteVirtualMemory(NtCurrentProcess(),
		(PVOID)pZwContinue,
		&hookbuf,
		sizeof(hookbuf),
		&returnLen);
	if (!NT_SUCCESS(status))
	{
		goto __exit;
	}
	trace = 10;


__exit:
	DPRINT("%s TRACE:%d status = %08X \n", __FUNCTION__, trace, status);
	if (attach) { KeUnstackDetachProcess(&apc); }
	ExFreePool(StartContext);
//	ExFreeToNPagedLookasideList(&g_injectDataLookaside, StartContext);
	PsTerminateSystemThread(0);

}

VOID LoadImageNotify(
	_In_ PUNICODE_STRING FullImageName,
	_In_ HANDLE ProcessId,
	_In_ PIMAGE_INFO ImageInfo
)
{
	//
	//过滤system进程
	//

	if (FullImageName == NULL ||
		ProcessId == (HANDLE)4 ||
		ProcessId == (HANDLE)0 ||
		ImageInfo == NULL ||
		ImageInfo->SystemModeImage == 1)
	{
		return;
	}

	if (KeGetCurrentIrql() != PASSIVE_LEVEL)
	{
		return;
	}

	BOOLEAN		x64Process = false;


	PEPROCESS	process = NULL;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(ProcessId, &process)))
	{
		return;
	}

	x64Process = (PsGetProcessWow64Process(process) == NULL);
	PUNICODE_STRING ProcessName = nullptr;
	if (NT_SUCCESS(SeLocateProcessImageName(process, &ProcessName)))
	{
		//UNICODE_STRING		TagName = { 0 };
		//RtlInitUnicodeString(&TagName, L"LEAGUE OF LEGENDS.EXE");
		//DPRINT("[-]ProcessName %ws\n", ProcessName->Buffer);
		if (std::wstring(ProcessName->Buffer).find(L"League of Legends.exe")==std::wstring::npos)
		{
			ObDereferenceObject(process);
			return;
		}
	}
	else
	{
		ObDereferenceObject(process);
		return;
	}

	ObDereferenceObject(process);



	if (x64Process)
	{
		UNICODE_STRING	ntdll_fullimage;
		RtlInitUnicodeString(&ntdll_fullimage, L"\\System32\\ntdll.dll");
		if (SafeSearchString(FullImageName, &ntdll_fullimage, TRUE) == -1)
		{
			return;
		}
	}
	else
	{
		UNICODE_STRING	ntdll_fullimage;
		RtlInitUnicodeString(&ntdll_fullimage, L"\\SysWOW64\\ntdll.dll");

		if (SafeSearchString(FullImageName, &ntdll_fullimage, TRUE) == -1)
		{
			return;
		}
	}

	//
	//开始注入
	//

	NTSTATUS	status;
	HANDLE		thread_hanlde;
	PVOID		thread_object;
	PINJECT_PROCESSID_DATA	injectdata = (PINJECT_PROCESSID_DATA)ExAllocatePool(NonPagedPool, sizeof(PINJECT_PROCESSID_DATA));

	if (injectdata == NULL)
	{
		return;
	}

	injectdata->pid = ProcessId;
	injectdata->imagebase = ImageInfo->ImageBase;
	injectdata->imagesize = ImageInfo->ImageSize;

	status = PsCreateSystemThread(
		&thread_hanlde,
		THREAD_ALL_ACCESS,
		NULL,
		NtCurrentProcess(),
		NULL,
		 INJECT_ROUTINE_X86,
		injectdata);
	if (NT_SUCCESS(status))
	{
		//添加到已经注入列表里面
		//SetInjectListStatus(ProcessId);

		if (NT_SUCCESS(ObReferenceObjectByHandle(thread_hanlde, THREAD_ALL_ACCESS, NULL, KernelMode, &thread_object, NULL)))
		{

			KeWaitForSingleObject(thread_object, Executive, KernelMode, FALSE, NULL);

			ObDereferenceObject(thread_object);
		}

		ZwClose(thread_hanlde);
		ExQueueWorkItem(g_pUnloadWorkItem, DelayedWorkQueue);
	}




}
VOID DriverUnload(
	IN PDRIVER_OBJECT DriverObject)
{
	PsRemoveLoadImageNotifyRoutine(LoadImageNotify);
}


EXTERN_C  NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString)
{
	PDEVICE_OBJECT DeviceObject = NULL;
	pDriverObj->DriverUnload = DriverUnload;

	pfn_NtProtectVirtualMemory = static_cast<fn_NtProtectVirtualMemory>(SSDT::GetSSDTFunctionByName("NtProtectVirtualMemory"));
	pfn_NtReadVirtualMemory = static_cast<fn_NtReadVirtualMemory>(SSDT::GetSSDTFunctionByName("NtReadVirtualMemory"));
	pfn_NtWriteVirtualMemory = static_cast<fn_NtWriteVirtualMemory>(SSDT::GetSSDTFunctionByName("NtWriteVirtualMemory"));
	pfn_NtAllocateVirtualMemory = static_cast<decltype(pfn_NtAllocateVirtualMemory)>(SSDT::GetSSDTFunctionByName("NtAllocateVirtualMemory"));
	if (pfn_NtProtectVirtualMemory == nullptr || pfn_NtReadVirtualMemory == nullptr || pfn_NtWriteVirtualMemory == nullptr || pfn_NtAllocateVirtualMemory == nullptr)
	{
		return STATUS_UNSUCCESSFUL;
	}
	g_pRegistryPath = (PUNICODE_STRING)ExAllocatePool(NonPagedPool, sizeof(UNICODE_STRING));
	g_pRegistryPath->Buffer = (PWCH)ExAllocatePool(NonPagedPool, pRegistryString->MaximumLength);
	g_pRegistryPath->Length = pRegistryString->Length;
	g_pRegistryPath->MaximumLength = pRegistryString->MaximumLength;
	memcpy(g_pRegistryPath->Buffer, pRegistryString->Buffer, g_pRegistryPath->Length);

	g_pUnloadWorkItem = (PWORK_QUEUE_ITEM)ExAllocatePool(NonPagedPool, sizeof(WORK_QUEUE_ITEM));
	ExInitializeWorkItem(g_pUnloadWorkItem, (PWORKER_THREAD_ROUTINE)ZwUnloadDriver, g_pRegistryPath);
	 PsSetLoadImageNotifyRoutine(LoadImageNotify);
	return	STATUS_SUCCESS;
}