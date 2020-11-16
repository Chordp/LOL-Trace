#include "PE.h"
#include <string>
//进程环境块（因为Windows内核有一个机构PEB，为了不重定义，所以就另起一个名字）


namespace PE
{
	PIMAGE_NT_HEADERS64 GetNtHeaders(PVOID image_base)
	{
		const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(image_base);

		if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
			return nullptr;

		const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS64>(reinterpret_cast<ULONG64>(image_base) + dos_header->e_lfanew);

		if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
			return nullptr;

		return nt_headers;
	}

	ULONG RvaToFoa(PIMAGE_NT_HEADERS pNtHeader, ULONG Rva, ULONG FileSize)
	{
		PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
		USHORT NumberOfSections = pNtHeader->FileHeader.NumberOfSections;
		ULONG Foa = ERROR_VALUE;
		for (int i = 0; i < NumberOfSections; i++)
		{
			if (Rva >= pSectionHeader->VirtualAddress && Rva < (pSectionHeader->VirtualAddress + pSectionHeader->Misc.VirtualSize))
			{
				Foa = Rva - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;//FOA=RVA-VA+RAW
				if (Foa <= FileSize)
					return Foa;
			}
			pSectionHeader++;
		}
		return ERROR_VALUE;
	}

	// 获取导出函数在文件数据中的代码位置偏移
	ULONG GetExportFoa(unsigned char* FileData, ULONG FileSize, char* ExportName)
	{

		// 4D5A
		PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)FileData;
		if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		{
			return ERROR_VALUE;
		}
		// PE
		PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(FileData + pDosHeader->e_lfanew);
		if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
		{
			return ERROR_VALUE;
		}
		PIMAGE_DATA_DIRECTORY pDataDirectory = NULL;
		if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)//64bit PE
		{
			pDataDirectory = ((PIMAGE_NT_HEADERS64)pNtHeader)->OptionalHeader.DataDirectory;
		}
		else
		{
			pDataDirectory = ((PIMAGE_NT_HEADERS32)pNtHeader)->OptionalHeader.DataDirectory;
		}

		ULONG ExportDirRva = pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
		ULONG ExportDirSize = pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
		ULONG ExportDirFoa = RvaToFoa(pNtHeader, ExportDirRva, FileSize);
		if (ExportDirFoa == ERROR_VALUE)
		{
			//LOGDEBUG(ExportDirFoa, "Export Directory Not Found");
			return ERROR_VALUE;
		}
		//Read Export Directory
		PIMAGE_EXPORT_DIRECTORY pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(FileData + ExportDirFoa);
		ULONG NumberOfNames = pExportDirectory->NumberOfNames;
		ULONG AddressOfFunctionsOffset = RvaToFoa(pNtHeader, pExportDirectory->AddressOfFunctions, FileSize);
		ULONG AddressOfNameOrdinalsOffset = RvaToFoa(pNtHeader, pExportDirectory->AddressOfNameOrdinals, FileSize);
		ULONG AddressOfNamesOffset = RvaToFoa(pNtHeader, pExportDirectory->AddressOfNames, FileSize);
		if (AddressOfFunctionsOffset == ERROR_VALUE || AddressOfNameOrdinalsOffset == ERROR_VALUE || AddressOfNamesOffset == ERROR_VALUE)
		{
			//LOGDEBUG(KERNELDRAW_UNSUCCESSFUL, "Export Directory Contents Error");
			return ERROR_VALUE;
		}
		ULONG* AddressOfFunctions = (ULONG*)(FileData + AddressOfFunctionsOffset);
		USHORT* AddressOfNameOrdinals = (USHORT*)(FileData + AddressOfNameOrdinalsOffset);
		ULONG* AddressOfNames = (ULONG*)(FileData + AddressOfNamesOffset);

		//Find Export
		ULONG ExportFoa = ERROR_VALUE;
		for (ULONG i = 0; i < NumberOfNames; i++)
		{
			ULONG CurrentNameOffset = RvaToFoa(pNtHeader, AddressOfNames[i], FileSize);
			if (CurrentNameOffset == ERROR_VALUE)
				continue;
			const char* CurrentName = (const char*)(FileData + CurrentNameOffset);
			ULONG CurrentFunctionRva = AddressOfFunctions[AddressOfNameOrdinals[i]];
			//DPRINT("%s\n", CurrentName);

			if (CurrentFunctionRva >= ExportDirRva && CurrentFunctionRva < ExportDirRva + ExportDirSize)
				continue; //we ignore forwarded exports
			if (!_stricmp(CurrentName, ExportName))  //compare the export name to the requested export
			{
				ExportFoa = RvaToFoa(pNtHeader, CurrentFunctionRva, FileSize);
				break;
			}
		}
		if (ExportFoa == ERROR_VALUE)
		{
			KdPrint(("Export %s not found in export table!\r\n", ExportName));
		}

		return ExportFoa;
	}

}