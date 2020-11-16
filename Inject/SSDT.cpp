#include "SSDT.h"
#include "Process.h"
#include "PE.h"
struct SSDTStruct
{
    LONG* pServiceTable;
    PVOID pCounterTable;
#ifdef _WIN64
    ULONGLONG NumberOfServices;
#else
    ULONG NumberOfServices;
#endif
    PCHAR pArgumentTable;
};
SSDTStruct* SSDTfind()
{
    static SSDTStruct* SSDT = 0;
    if (!SSDT)
    {
#ifndef _WIN64
        //x86 code
        UNICODE_STRING routineName;
        RtlInitUnicodeString(&routineName, L"KeServiceDescriptorTable");
        SSDT = (SSDTStruct*)MmGetSystemRoutineAddress(&routineName);
#else
        //x64 code
        size_t kernelSize;
        ULONG_PTR kernelBase = (ULONG_PTR)Process::GetKernelModuleAddress("ntoskrnl.exe",kernelSize);
        if (kernelBase == 0 || kernelSize == 0)
            return NULL;

        // Find KiSystemServiceStart
        const unsigned char KiSystemServiceStartPattern[] = { 0x8B, 0xF8, 0xC1, 0xEF, 0x07, 0x83, 0xE7, 0x20, 0x25, 0xFF, 0x0F, 0x00, 0x00 };
        const ULONG signatureSize = sizeof(KiSystemServiceStartPattern);
        bool found = false;
        ULONG KiSSSOffset;
        for (KiSSSOffset = 0; KiSSSOffset < kernelSize - signatureSize; KiSSSOffset++)
        {
            if (RtlCompareMemory(((unsigned char*)kernelBase + KiSSSOffset), KiSystemServiceStartPattern, signatureSize) == signatureSize)
            {
                found = true;
                break;
            }
        }
        if (!found)
            return NULL;

        // lea r10, KeServiceDescriptorTable
        ULONG_PTR address = kernelBase + KiSSSOffset + signatureSize;
        LONG relativeOffset = 0;
        if ((*(unsigned char*)address == 0x4c) &&
            (*(unsigned char*)(address + 1) == 0x8d) &&
            (*(unsigned char*)(address + 2) == 0x15))
        {
            relativeOffset = *(LONG*)(address + 3);
        }
        if (relativeOffset == 0)
            return NULL;

        SSDT = (SSDTStruct*)(address + relativeOffset + 7);
#endif
    }
    return SSDT;
}
NTSTATUS  ReadNtdllFileData(unsigned char** pFileData, PULONG pFileSize)
{
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    unsigned char* FileData = NULL;
    ULONG FileSize = NULL;

    // Init FileObject
    UNICODE_STRING FileName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    RtlInitUnicodeString(&FileName, L"\\SystemRoot\\system32\\ntdll.dll");
    InitializeObjectAttributes(&ObjectAttributes, &FileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    if (KeGetCurrentIrql() != PASSIVE_LEVEL)
    {
        return STATUS_UNSUCCESSFUL;
    }

    // 打开文件
    HANDLE FileHandle;
    IO_STATUS_BLOCK IoStatusBlock;
    status = ZwCreateFile(&FileHandle,
        GENERIC_READ,
        &ObjectAttributes,
        &IoStatusBlock, NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ,
        FILE_OPEN,
        FILE_SYNCHRONOUS_IO_NONALERT,
        NULL, 0);
    if (NT_SUCCESS(status))
    {
        FILE_STANDARD_INFORMATION StandardInformation = { 0 };
        status = ZwQueryInformationFile(FileHandle, &IoStatusBlock, &StandardInformation, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);
        if (NT_SUCCESS(status))
        {
            // 文件大小
            FileSize = StandardInformation.EndOfFile.LowPart;
            //DPRINT("win32k.dll FileSize\n", StandardInformation.EndOfFile.LowPart);
            //LOGDEBUG(StandardInformation.EndOfFile.LowPart, "win32k.dll FileSize");

            // 申请空间
            FileData = (UCHAR*)ExAllocatePoolWithTag(NonPagedPool, FileSize, 'ex');
            if (FileData == NULL)
                return STATUS_NO_PA_DATA;

            RtlZeroMemory(FileData, FileSize);

            // 读取文件
            LARGE_INTEGER ByteOffset;
            ByteOffset.LowPart = ByteOffset.HighPart = 0;
            status = ZwReadFile(FileHandle,
                NULL, NULL, NULL,
                &IoStatusBlock,
                FileData,
                FileSize,
                &ByteOffset, NULL);

            if (!NT_SUCCESS(status))
            {
                //LOGDEBUG(status, "Read win32k.dll Fail status");
                ExFreePool(FileData);
            }
        }
        else
        {
            //LOGDEBUG(status, "ZwQueryInformationFile Fail status");
        }
        ZwClose(FileHandle);
    }
    else
    {
        //LOGDEBUG(status, "ZwCreateFile failed  status");
    }
    // 传出
    *pFileData = FileData;
    *pFileSize = FileSize;
    //LOGDEBUG(FileData, "win32k.dll FileData");
    return status;
}
int GetExportSsdtIndex(std::string ExportName)
{
    unsigned char* FileData = NULL;
    ULONG FileSize = NULL;
    auto status = ReadNtdllFileData(&FileData, &FileSize);
    if (NT_SUCCESS(status))
    {
        ULONG_PTR ExportOffset = PE::GetExportFoa(FileData, FileSize, (char*)ExportName.c_str());
        if (ExportOffset == ERROR_VALUE)
            return -1;

        int SsdtOffset = -1;
        unsigned char* ExportData = FileData + ExportOffset;
        for (int i = 0; i < 32 && ExportOffset + i < FileSize; i++)
        {
            if (ExportData[i] == 0xC2 || ExportData[i] == 0xC3)  //RET
                break;
            if (ExportData[i] == 0xB8)  //mov eax,X
            {
                SsdtOffset = *(int*)(ExportData + i + 1);
                break;
            }
        }

        if (SsdtOffset == -1)
        {
            DPRINT("[DeugMessage] SSDT Offset for %s not found...\r\n", ExportName);
        }

        if (FileData)
            ExFreePool(FileData);
        return SsdtOffset;

        
    }
    return ERROR_VALUE;
}
PVOID SSDT::GetSSDTFunctionByName(std::string name)
{
    //read address from SSDT
    SSDTStruct* SSDT = SSDTfind();
    if (!SSDT)
    {
        DPRINT("[DeugMessage] SSDT not found...\r\n");
        return 0;
    }
    ULONG_PTR SSDTbase = (ULONG_PTR)SSDT->pServiceTable;
    if (!SSDTbase)
    {
        DPRINT("[DeugMessage] ServiceTable not found...\r\n");
        return 0;
    }
    ULONG readOffset = GetExportSsdtIndex(name.c_str());
    if (readOffset == -1)
        return 0;
    if (readOffset >= SSDT->NumberOfServices)
    {
        DPRINT("[DeugMessage] Invalid read offset...\r\n");
        return 0;
    }
#ifdef _WIN64
    return (PVOID)((SSDT->pServiceTable[readOffset] >> 4) + SSDTbase);
#else
    return (PVOID)SSDT->pServiceTable[readOffset];
#endif
}