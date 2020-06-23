#pragma once
#include  "Trace.h"
#include "Detour.hpp"
enum HookType
{
	Vmt,
	Inline
};


//template <auto fun>
//class MakeHookF
//{
//private:
//	using type = decltype(fun);
//	bool _isApplied;
//public:
//	DetourSb Instance;
//	type _orig;
//	type _detour;
//	template <typename T>
//	void Apply(T pFunc, HookType hook_type ,type detour)
//	{
//		switch (hook_type)
//		{
//		case Vmt:
//			break;
//		case Inline:
//		{
//			Instance = DetourSb(pFunc, detour);
//			_detour = detour;
//			_orig  = Instance.Install();
//
//			_isApplied = _orig != nullptr;
//			break;
//		}
//		default:
//			break;
//		}
//	}
//
//
//	bool Remove()
//	{
//		if (!_isApplied)
//			return false;
//
//		_isApplied = false;
//
//		return Instance.Remove();
//
//	}
//
//	template <typename ...args>
//	auto CallOriginal(args ... p)
//	{
//		return _orig(p...);
//	}
//
//	template <typename ...args>
//	auto CallDetour(args ... p)
//	{
//		return _detour(p...);
//	}
//
//	bool IsApplied() const
//	{
//		return _isApplied;
//	}
//};
template <typename retn, typename ...args>
class MakeHook
{
private:
	using type = retn(__stdcall*)(args...);
	bool _isApplied;
public:
	Detour Instance;
	type _orig;
	type _detour;
	template <typename T>
	void Apply(T pFunc, HookType hook_type, type detour)
	{
		switch (hook_type)
		{
		case Vmt:
			break;
		case Inline:
		{
			Instance = Detour((PVOID)pFunc, detour);
			_detour = detour;
			_orig = static_cast<type>(Instance.Install());

			_isApplied = _orig != nullptr;
			break;
		}
		default:
			break;
		}
	}


	bool Remove()
	{
		if (!_isApplied)
			return false;

		_isApplied = false;

		return Instance.Remove();

	}

	auto CallOriginal(args ... p)
	{
		return _orig(p...);
	}


	auto CallDetour(args ... p)
	{
		return _detour(p...);
	}

	bool IsApplied() const
	{
		return _isApplied;
	}
};