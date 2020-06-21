#pragma once
#include "SmartHook.hpp"
#include "detours.h"


class Detour
{
private:
	PVOID _pFunc{};
	PVOID _detour{};
public:
	Detour(){};
	Detour(PVOID pFunc, PVOID detour) :_pFunc(pFunc), _detour(detour) {};
	PVOID Install() const
	{
		return DetourFunction(static_cast<PBYTE>(_pFunc), static_cast<PBYTE>(_detour));
	}
	auto Remove() const
	{
		return DetourRemove(static_cast<PBYTE>(_pFunc), static_cast<PBYTE>(_detour));
	}
};
