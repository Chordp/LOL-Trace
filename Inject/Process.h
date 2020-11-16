#pragma once
#include "Main.h"
#include <string>
#include <vector>
#include "PEStruct.h"
typedef struct _ProcessStruct
{
	HANDLE PID = nullptr;
	std::wstring ProcessName;
}ProcessStruct, * pProcessStruct;
EXTERN_C NTKERNELAPI  PPEB_EX  PsGetProcessPeb(PEPROCESS Process);
typedef PETHREAD(NTAPI* PSGETNEXTPROCESSTHREAD) (IN PEPROCESS, IN PETHREAD OPTIONAL);
enum Alloc_class
{
	PT,
	MDLX,
	MAP
};
class Process
{
public:

	//Process(HANDLE PID):_PID(PID) {};
	Process(PEPROCESS EPROCESS) : _EProcess(EPROCESS) {};
	Process() {};

	VOID	ReadMemory(ULONG64 address, PVOID buffer, SIZE_T size);
	VOID	WriteMemory(ULONG64 address, PVOID buffer, SIZE_T size);
	VOID	ZeroMemory(ULONG64 address, SIZE_T size);
	template <class T> T	READ(ULONG64 address)
	{

		T buffer{};
		ReadMemory(address, &buffer, sizeof(T));
		return buffer;
	}
	template <class T> void  Write(ULONG64 address, T buffer)
	{
		WriteMemory(address, &buffer, sizeof(T));
	}
	PVOID AllocMemory(Alloc_class, size_t);
	static 	HANDLE GetProcessIdByName(std::wstring ModuleName);
	static	PVOID  GetModuleExport(HANDLE ProcessId, IN PVOID pBase, IN PCCHAR name_ord);
	static  HANDLE GetProcessModule(HANDLE ProcessId, std::wstring ModuleName);

	void	PteProtext(PVOID pAddress);
	static  PVOID  GetModuleExport(IN PVOID pBase, IN PCCHAR name_ord);
	static  PVOID  GetKernelModuleAddress(const std::string& module_name, size_t& len);
	static	PVOID  GetProcessWin32Thread(HANDLE PID);
	static  HANDLE EnumTlsGame();
	static  std::vector<ProcessStruct>  EnumProcess();

private:
	HANDLE hMemory = (HANDLE)-1;
	PEPROCESS	_EProcess;
};

