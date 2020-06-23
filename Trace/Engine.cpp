#include "Engine.hpp"

DWORD_PTR Engine::GetBaseModule()
{
	 auto Base = reinterpret_cast<DWORD_PTR> (GetModuleHandleA(nullptr));
	return Base;
}

HWND Engine::GetWindow()
{
	 auto hWnd = FindWindowA(0, "League of Legends (TM) Client");

	return hWnd;
}

 Hero* Engine::GetLocalPlayer()
{
	return *reinterpret_cast<Hero**>(GetBaseModule() + ObjectManager::LocalPlayer);
}
 float Engine::GetGameTime()
{
	return *(float*)(GetBaseModule() + GameInfo::GameTime);
}
 bool Engine::WorldToScreen(Vector* World, Vector* Screen)
{
	 auto fnW2S = reinterpret_cast<bool(__cdecl*)(Vector*, Vector*)>(GetBaseModule() + Function::WorldToScreen);
	return fnW2S(World, Screen);
}
 bool Engine::WorldToScreen(Vector& World, Vector& Screen)
{
	return Engine::WorldToScreen(&World, &Screen);
}

  char* Engine::GetBuildVersion()
 {
	 return (char*)(GetBaseModule() + GameInfo::BuildVersion);
 }
 void Engine::PrintChat(DWORD ChatClient, const char* Massage, int Color)
{
	 auto fnPrintChat = reinterpret_cast<void(__thiscall*)(DWORD, const char*, int)>(GetBaseModule());
	return fnPrintChat(ChatClient, Massage, Color);
}