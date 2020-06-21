#pragma once
#include "Trace.h"
class Engine
{
private:
	
	
public:
	static auto GetBaseModule()
	{
		static auto Base = reinterpret_cast<DWORD_PTR> (GetModuleHandleA(nullptr));
		return Base;
	}

	static auto GetGameWindow()
	{
		static auto hWnd  = FindWindowA(0, "League of Legends (TM) Client");

		return hWnd;
	}
	static bool WorldToScreen(Vector* World,Vector*Screen)
	{
		static auto fnW2S = reinterpret_cast<bool(__cdecl*)(Vector*, Vector*)>(GetBaseModule());
		return fnW2S(World, Screen);
	}
	static bool WorldToScreen(Vector& World,Vector& Screen)
	{
		return WorldToScreen(&World, &Screen);
	}
	static bool WorldToScreen(Vector* World, Vector& Screen)
	{
		return WorldToScreen(World, &Screen);
	}
	static bool WorldToScreen(Vector World, Vector& Screen)
	{
		return WorldToScreen(&World, &Screen);
	}
	static void PrintChat(DWORD ChatClient ,const char* Massage ,int Color)
	{
		static auto fnPrintChat = reinterpret_cast<void(__thiscall*)(DWORD, const char*, int)>(GetBaseModule());
		return fnPrintChat(ChatClient, Massage, Color);
	}
};

