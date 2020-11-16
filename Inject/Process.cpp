#include "Process.h"
#include "PEStruct.h"
#include <intrin.h>

std::vector<ProcessStruct> Process::EnumProcess()
{
	auto						Status = STATUS_SUCCESS;

	std::vector<ProcessStruct>	hRet;

	PVOID						pBuffer = nullptr;

	DWORD						buffer_size = NULL;


	Status = ZwQuerySystemInformation(SystemProcessInformation, pBuffer, 0, &buffer_size);
	while (Status == STATUS_INFO_LENGTH_MISMATCH)
	{
		if (pBuffer)	ExFreePool(pBuffer);

		pBuffer = ExAllocatePool(NonPagedPool, buffer_size);
		Status = ZwQuerySystemInformation(SystemProcessInformation, pBuffer, buffer_size, &buffer_size);


	}

	auto	ProcessInformation = static_cast<PSYSTEM_PROCESS_INFORMATION>(pBuffer);
	for (;;)
	{

		if (ProcessInformation->NextEntryOffset == 0)
			break;
		hRet.push_back(ProcessStruct{ ProcessInformation->ProcessId, ProcessInformation->ImageName.Buffer });
		ProcessInformation = (PSYSTEM_PROCESS_INFORMATION)(((PUCHAR)ProcessInformation) + ProcessInformation->NextEntryOffset);
	}
	ExFreePool(pBuffer);
	return hRet;

}
HANDLE	Process::GetProcessIdByName(std::wstring ModuleName)
{
	auto				Status = STATUS_SUCCESS;
	UNICODE_STRING		TagName = { 0 };

	HANDLE				hRet = nullptr;

	PVOID				pBuffer = nullptr;

	DWORD				buffer_size = NULL;
	RtlInitUnicodeString(&TagName, ModuleName.c_str());


	Status = ZwQuerySystemInformation(SystemProcessInformation, pBuffer, 0, &buffer_size);
	while (Status == STATUS_INFO_LENGTH_MISMATCH)
	{
		if (pBuffer)	ExFreePool(pBuffer);

		pBuffer = ExAllocatePool(NonPagedPool, buffer_size);
		Status = ZwQuerySystemInformation(SystemProcessInformation, pBuffer, buffer_size, &buffer_size);


	}

	auto	ProcessInformation = static_cast<PSYSTEM_PROCESS_INFORMATION>(pBuffer);
	for (;;)
	{

		//DPRINT("%ws\n", ProcessInformation->ImageName.Buffer);
		if (FsRtlIsNameInExpression(&TagName, &(ProcessInformation->ImageName), FALSE, NULL) == TRUE)
		{
			//DbgBreakPoint();
			hRet = ProcessInformation->ProcessId;
			break;
		}

		if (ProcessInformation->NextEntryOffset == 0)
			break;
		ProcessInformation = (PSYSTEM_PROCESS_INFORMATION)(((PUCHAR)ProcessInformation) + ProcessInformation->NextEntryOffset);
	}
	ExFreePool(pBuffer);
	return hRet;
}
PVOID Process::GetProcessWin32Thread(HANDLE PID)
{
	auto				Status = STATUS_SUCCESS;


	PVOID				hRet = nullptr;

	PVOID				pBuffer = nullptr;

	DWORD				buffer_size = NULL;


	Status = ZwQuerySystemInformation(SystemProcessInformation, pBuffer, 0, &buffer_size);
	while (Status == STATUS_INFO_LENGTH_MISMATCH)
	{
		if (pBuffer)	ExFreePool(pBuffer);

		pBuffer = ExAllocatePool(NonPagedPool, buffer_size);
		Status = ZwQuerySystemInformation(SystemProcessInformation, pBuffer, buffer_size, &buffer_size);


	}

	auto	ProcessInformation = static_cast<PSYSTEM_PROCESS_INFORMATION2>(pBuffer);
	for (;;)
	{

		if (PID == ProcessInformation->ProcessId)
		{
			for (size_t i = 0; i < ProcessInformation->ThreadCount; i++)
			{

				PSYSTEM_THREAD_INFORMATION ThreadInf = (PSYSTEM_THREAD_INFORMATION)&ProcessInformation->Threads[i];


				PETHREAD EThread;
				NTSTATUS status = PsLookupThreadByThreadId(ThreadInf->ClientId.UniqueThread, &EThread);
				if (NT_SUCCESS(status))
				{
					ObDereferenceObject(EThread);

					auto Win32KThread = PsGetThreadWin32Thread(EThread);
					if (Win32KThread != nullptr)
					{
						hRet = Win32KThread;
						//return Win32KThread;
						goto Exit;
					}
				}
			}

		}

		if (ProcessInformation->NextEntryDelta == 0)
			break;
		ProcessInformation = (PSYSTEM_PROCESS_INFORMATION2)(((PUCHAR)ProcessInformation) + ProcessInformation->NextEntryDelta);
	}
Exit:
	ExFreePool(pBuffer);
	return hRet;
}


HANDLE	Process::EnumTlsGame()
{

	auto				Status = STATUS_SUCCESS;
	UNICODE_STRING		TagName = { 0 };

	HANDLE				hRet = nullptr;

	PVOID				pBuffer = nullptr;

	DWORD				buffer_size = NULL;



	RtlInitUnicodeString(&TagName, L"TslGame.exe");


	Status = ZwQuerySystemInformation(SystemProcessInformation, pBuffer, 0, &buffer_size);
	while (Status == STATUS_INFO_LENGTH_MISMATCH)
	{
		if (pBuffer)	ExFreePool(pBuffer);

		pBuffer = ExAllocatePool(NonPagedPool, buffer_size);
		Status = ZwQuerySystemInformation(SystemProcessInformation, pBuffer, buffer_size, &buffer_size);


	}

	auto	ProcessInformation = static_cast<PSYSTEM_PROCESS_INFORMATION>(pBuffer);
	for (;;)
	{
		if (FsRtlIsNameInExpression(&TagName, &(ProcessInformation->ImageName), FALSE, NULL) == TRUE)
		{

			PEPROCESS EPROCESS = NULL;

			Status = PsLookupProcessByProcessId(ProcessInformation->InheritedFromProcessId, &EPROCESS);
			if (!NT_SUCCESS(Status))
			{
				break;
			}

			if (_stricmp(PsGetProcessImageFileName(EPROCESS), "TslGame.exe") == 0)
			{
				hRet = ProcessInformation->ProcessId;
				ObDereferenceObject(EPROCESS);
				break;
			}
			ObDereferenceObject(EPROCESS);
		}

		if (ProcessInformation->NextEntryOffset == 0)
			break;
		ProcessInformation = (PSYSTEM_PROCESS_INFORMATION)(((PUCHAR)ProcessInformation) + ProcessInformation->NextEntryOffset);
	}
	ExFreePool(pBuffer);
	return hRet;

}

PVOID	Process::GetKernelModuleAddress(const std::string& module_name, size_t& len)
{
	void* buffer = nullptr;
	DWORD buffer_size = 0;
	// NtQuerySystemInformation = (NTQUERYSYSTEMINFORMATION)MmGetSystemRoutineAddress(&routineName);
	//DbgBreakPoint();
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, buffer, buffer_size, &buffer_size);

	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		if (buffer)	ExFreePool(buffer);

		buffer = ExAllocatePool(NonPagedPool, buffer_size);
		status = ZwQuerySystemInformation(SystemModuleInformation, buffer, buffer_size, &buffer_size);
	}



	if (!NT_SUCCESS(status))
	{
		ExFreePool(buffer);
		return 0;
	}
	const auto modules = static_cast<PRTL_PROCESS_MODULES>(buffer);

	for (auto i = 0u; i < modules->NumberOfModules; ++i)
	{
		const std::string current_module_name = std::string(reinterpret_cast<char*>(modules->Modules[i].FullPathName) + modules->Modules[i].OffsetToFileName);
		if (current_module_name == module_name)
		{
			const PVOID result = modules->Modules[i].ImageBase;
			len = modules->Modules[i].ImageSize;

			return result;
		}
	}
	return 0;
}
HANDLE	Process::GetProcessModule(HANDLE ProcessId, std::wstring ModuleName)
{
	KAPC_STATE APC;
	HANDLE nRet = nullptr;
	UNICODE_STRING                 moudleName = { 0 };
	PPEB_EX Peb;
	PEPROCESS EProcess;
	auto status = PsLookupProcessByProcessId(ProcessId, &EProcess);
	if (NT_SUCCESS(status))
	{
		Peb = PsGetProcessPeb(EProcess);
		if (!Peb)
			return nullptr;
		RtlInitUnicodeString(&moudleName, ModuleName.c_str());
		KeStackAttachProcess(EProcess, &APC);


		PPEB_LDR_DATA_EX peb_LDR_data = (PPEB_LDR_DATA_EX)Peb->Ldr;

		PLIST_ENTRY list_entry = &peb_LDR_data->InLoadOrderModuleList;
		PLIST_ENTRY currentList = list_entry->Flink;

		while (currentList != list_entry)

		{

			PLDR_DATA_TABLE_ENTRY_EX ldr_data_table_entry = (PLDR_DATA_TABLE_ENTRY_EX)currentList;
			//DPRINT("%ws\n", ldr_data_table_entry->BaseDllName.Buffer);
			
			if (FsRtlIsNameInExpression(&moudleName, &ldr_data_table_entry->BaseDllName, TRUE, NULL))
			{
				nRet = ldr_data_table_entry->DllBase;
				break;
			}



			//指向下一个

			currentList = currentList->Flink;

		}
		KeUnstackDetachProcess(&APC);
		ObDereferenceObject(EProcess);
	}
	return nRet;

}



PVOID	Process::GetModuleExport(HANDLE ProcessId, IN PVOID pBase, IN PCCHAR name_ord)
{

	PEPROCESS EProcess;
	PVOID hRet = nullptr;
	auto status = PsLookupProcessByProcessId(ProcessId, &EProcess);
	if (NT_SUCCESS(status))
	{
		KAPC_STATE apc = { 0 };
		KeStackAttachProcess(EProcess, &apc);
		hRet = GetModuleExport(pBase, name_ord);
		KeUnstackDetachProcess(&apc);
		ObDereferenceObject(EProcess);
	}
	return hRet;
}
ULONG64 MiInitPteBase()
{

	ULONG64 vStatus = 0;

	for (;;)
	{
		char sSearchPatten[] =
		{
			// mov     rax,[rax + rdx * 8]
			// shl     rax, 19h
			// mov     rdx, PTE_BASE

			0x48, 0x8B, 0x04, 0xD0,
			0x48, 0xC1, 0xE0, 0x19,
			0x48, 0xBA, // 00 00 00 00 80 F6 FF FF
		};
		UNICODE_STRING routineName;
		RtlInitUnicodeString(&routineName, L"MmGetVirtualForPhysical");

		PUCHAR vSearchAddress = (PUCHAR)MmGetSystemRoutineAddress(&routineName);
		if (0 == vSearchAddress)
		{
			vStatus = 0;
			break;
		}

		ULONG64 vHitAddress = 0;
		for (int i = 0; i < 0x60; ++i)
		{
			if (sizeof(sSearchPatten) == RtlCompareMemory(
				&(vSearchAddress[i]), sSearchPatten, sizeof(sSearchPatten)))
			{
				vHitAddress = (ULONG64) & (vSearchAddress[i]);
				//OutPut("<-->vHitAddress:%p\r\n", vHitAddress);
				break;
			}
		}
		if (0 == vHitAddress)
		{
			vStatus = 0x0FFFFF68000000000;
			if (!MmIsAddressValid((PVOID)vStatus)) {
				vStatus = 0;
			}
			break;
		}



		vStatus = *(PULONG64)(vHitAddress + sizeof(sSearchPatten));

		break;
	}

	return vStatus;
}
bool _MmIsAddressValidz(PVOID pAddress, ULONG64 pTE)
{
	//DbgBreakPoint();

	ULONG64* curItem;
	ULONG64 ulAddress = (ULONG64)pAddress;
	ULONG64 STA;
	ulAddress = (ULONG64)pAddress;
	ulAddress = (ulAddress >> 12) << 3;
	ulAddress = ulAddress & 0x7FFFFFFFF8;
	curItem = (ULONG64*)(pTE + ulAddress);
	if (MmIsAddressValid(curItem)) {
		ULONG64 itemTmp = *(ULONG64*)curItem;
		*(ULONG64*)curItem &= ~0x8000000000000000;
	}

	return TRUE;
}
ULONG64 Mm_MapToR3(PVOID address, size_t size) {
	//申请通信内存
	PVOID R3Address = 0;

	if (address == NULL) {
		return 0;
	}
	memset(address, 0, size);

	//MDL
	PMDL MD = IoAllocateMdl(address, size, FALSE, FALSE, NULL);
	if (MD == NULL) {
		return 0;
	}
	//刷星
	MmBuildMdlForNonPagedPool(MD);
	//映射到R3
	R3Address = MmMapLockedPagesSpecifyCache(MD, UserMode, MmCached, NULL, FALSE, NormalPagePriority);


	return (ULONG64)R3Address;
}

bool IsAddressSafe(PVOID Address, ULONG_PTR Size)
{
	PHYSICAL_ADDRESS PhyAddr = MmGetPhysicalAddress(Address);
	if (PhyAddr.QuadPart == 0)
	{
		return FALSE;
	}

	PVOID Temp = MmMapIoSpace(PhyAddr, Size, MmNonCached);

	if (Temp == NULL)
	{
		return FALSE;
	}

	MmUnmapIoSpace(Temp, Size);

	return TRUE;
}
void	Process::PteProtext(PVOID pAddress)
{
	KAPC_STATE  apc_state;
	KeStackAttachProcess(_EProcess, &apc_state);


	_MmIsAddressValidz((PVOID)pAddress, MiInitPteBase());
	KeUnstackDetachProcess(&apc_state);
}
PVOID	Process::AllocMemory(Alloc_class it, size_t size)
{
	PVOID hRet = nullptr;
	PHYSICAL_ADDRESS x;
	x.QuadPart = 0xFFFFFFFFFFFFFFFF;
	auto Address = MmAllocateContiguousMemory(size, x);
		// ExAllocatePool(NonPagedPoolExecute, size);
	ULONGLONG DwVirtualAddress = 0;
	NTSTATUS Status;

	if (Address)
	{
		RtlZeroMemory(Address, size);
		//DPRINT("%p\n", Address);
		//DbgBreakPoint();
		KAPC_STATE  apc_state;
		KeStackAttachProcess(_EProcess, &apc_state);
		switch (it)
		{
		case PT:
			break;
		case MDLX:
		{
			DwVirtualAddress = Mm_MapToR3(Address, size);
			if (DwVirtualAddress)
			{	
				auto pte = MiInitPteBase();
				for (ULONG64 Address = DwVirtualAddress; Address < DwVirtualAddress + 0x1000 * size; Address += 0x1000)
				{
					
					_MmIsAddressValidz((PVOID)Address, pte);
				}
				Status = STATUS_SUCCESS;
				 
			}
		}break;
		case MAP:
		{
			ULONGLONG DwAddress = (ULONGLONG)MmGetPhysicalAddress(Address).QuadPart;


			if (hMemory == (HANDLE)-1 || hMemory == (HANDLE)0) {
				UNICODE_STRING		physmemString;
				OBJECT_ATTRIBUTES	attributes;
				WCHAR				physmemName[] = L"\\device\\physicalmemory";
				NTSTATUS			status;
				HANDLE				physmem;
				RtlInitUnicodeString(&physmemString, physmemName);
				InitializeObjectAttributes(&attributes, &physmemString, OBJ_CASE_INSENSITIVE, NULL, NULL);
				status = ZwOpenSection(&physmem, SECTION_ALL_ACCESS, &attributes);
				if (!NT_SUCCESS(status))
				{
					goto Exit;
				}
				hMemory = physmem;
			}



			LARGE_INTEGER viewBase;
			viewBase.QuadPart = DwAddress;
			Status = ZwMapViewOfSection(hMemory, (HANDLE)-1, (void**)&DwVirtualAddress, 0L, size, &viewBase, &size, (SECTION_INHERIT)1, 0, PAGE_EXECUTE_READWRITE);
		}break;
		default:
			break;
		}


	Exit:
		KeUnstackDetachProcess(&apc_state);

	}
	return (Status == 0) ? (PVOID)DwVirtualAddress : NULL;
}
PVOID	Process::GetModuleExport(IN PVOID pBase, IN PCCHAR name_ord)
{
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)pBase;
	PIMAGE_NT_HEADERS32 pNtHdr32 = NULL;
	PIMAGE_NT_HEADERS64 pNtHdr64 = NULL;
	PIMAGE_EXPORT_DIRECTORY pExport = NULL;
	ULONG expSize = 0;
	ULONG_PTR pAddress = 0;
	PUSHORT pAddressOfOrds;
	PULONG  pAddressOfNames;
	PULONG  pAddressOfFuncs;
	ULONG i;
	//ASSERT(pBase != NULL);
	if (pBase == NULL)
		return NULL;
	/// Not a PE file
	if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
		return NULL;
	pNtHdr32 = (PIMAGE_NT_HEADERS32)((PUCHAR)pBase + pDosHdr->e_lfanew);
	pNtHdr64 = (PIMAGE_NT_HEADERS64)((PUCHAR)pBase + pDosHdr->e_lfanew);
	// Not a PE file
	if (pNtHdr32->Signature != IMAGE_NT_SIGNATURE)
		return NULL;
	// 64 bit image
	if (pNtHdr32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		pExport = (PIMAGE_EXPORT_DIRECTORY)(pNtHdr64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress + (ULONG_PTR)pBase);
		expSize = pNtHdr64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	}
	// 32 bit image
	else
	{
		pExport = (PIMAGE_EXPORT_DIRECTORY)(pNtHdr32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress + (ULONG_PTR)pBase);
		expSize = pNtHdr32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	}
	pAddressOfOrds = (PUSHORT)(pExport->AddressOfNameOrdinals + (ULONG_PTR)pBase);
	pAddressOfNames = (PULONG)(pExport->AddressOfNames + (ULONG_PTR)pBase);
	pAddressOfFuncs = (PULONG)(pExport->AddressOfFunctions + (ULONG_PTR)pBase);
	for (i = 0; i < pExport->NumberOfFunctions; ++i)
	{
		USHORT OrdIndex = 0xFFFF;
		PCHAR  pName = NULL;
		// Find by index
		if ((ULONG_PTR)name_ord <= 0xFFFF)
		{
			OrdIndex = (USHORT)i;
		}
		// Find by name
		else if ((ULONG_PTR)name_ord > 0xFFFF && i < pExport->NumberOfNames)
		{
			pName = (PCHAR)(pAddressOfNames[i] + (ULONG_PTR)pBase);
			OrdIndex = pAddressOfOrds[i];
		}
		// Weird params
		else
			return NULL;
		if (((ULONG_PTR)name_ord <= 0xFFFF && (USHORT)((ULONG_PTR)name_ord) == OrdIndex + pExport->Base) ||
			((ULONG_PTR)name_ord > 0xFFFF && strcmp(pName, name_ord) == 0))
		{

			pAddress = pAddressOfFuncs[OrdIndex] + (ULONG_PTR)pBase;
			// Check forwarded export
			if (pAddress >= (ULONG_PTR)pExport && pAddress <= (ULONG_PTR)pExport + expSize)
			{
				return NULL;
			}
			break;
		}
	}

	return (PVOID)pAddress;
}
VOID	Process::ReadMemory(ULONG64 address, PVOID buffer, SIZE_T size)
{
	KAPC_STATE  apc_state;
	KeStackAttachProcess(_EProcess, &apc_state);
	if (MmIsAddressValid((PVOID)address) && MmIsAddressValid((PVOID)(address + size)))
	//if (IsAddressSafe((PVOID)address, size))
	{
		
		RtlCopyMemory(buffer, (PVOID)address, size);
	}
	KeUnstackDetachProcess(&apc_state);
}
VOID	Process::WriteMemory(ULONG64 address, PVOID buffer, SIZE_T size)
{
	KAPC_STATE  apc_state;
	KeStackAttachProcess(_EProcess, &apc_state);
	if (MmIsAddressValid((PVOID)address) && MmIsAddressValid((PVOID)(address + size)))
	{
		KIRQL   tempirql = KeRaiseIrqlToDpcLevel();

		ULONG64  cr0 = __readcr0();

		cr0 &= 0xfffffffffffeffff;
		__writecr0(cr0);

		_disable();

		RtlCopyMemory((PVOID)address, buffer, size);

		cr0 = __readcr0();

		cr0 |= 0x10000;

		_enable();

		__writecr0(cr0);

		KeLowerIrql(tempirql);
	}
	KeUnstackDetachProcess(&apc_state);
}
VOID	Process::ZeroMemory(ULONG64 address, SIZE_T size)
{
	KAPC_STATE  apc_state;
	KeStackAttachProcess(_EProcess, &apc_state);
	if (MmIsAddressValid((PVOID)address) && MmIsAddressValid((PVOID)(address + size)))
	{
		
		KIRQL   tempirql = KeRaiseIrqlToDpcLevel();

		ULONG64  cr0 = __readcr0();

		cr0 &= 0xfffffffffffeffff;
		__writecr0(cr0);

		_disable();

		RtlZeroMemory((PVOID)address, size);

		cr0 = __readcr0();

		cr0 |= 0x10000;

		_enable();

		__writecr0(cr0);

		KeLowerIrql(tempirql);
	}
	KeUnstackDetachProcess(&apc_state);
}

