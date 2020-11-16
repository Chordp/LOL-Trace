#pragma once

#include "Main.h"
#include "PEStruct.h"
#include <string>
#include <vector>
#define ERROR_VALUE (ULONG)-1

namespace PE
{
	
	PIMAGE_NT_HEADERS64			GetNtHeaders(PVOID image_base);
	ULONG GetExportFoa(unsigned char* FileData, ULONG FileSize, char* ExportName);
}