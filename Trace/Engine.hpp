#pragma once
#include "Trace.h"
class Hero;
class Engine
{
private:
	
	
public:
	static DWORD_PTR GetBaseModule();

	static HWND GetWindow();

	static Hero* GetLocalPlayer();
	static inline char* GetBuildVersion();
	static float GetGameTime();
	static bool WorldToScreen(Vector* World, Vector* Screen);
	static bool WorldToScreen(Vector& World, Vector& Screen);
	static void PrintChat(DWORD ChatClient, const char* Massage, int Color);
};

