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
	return *reinterpret_cast<Hero**>(GetBaseModule() + GameClass::LocalPlayer);
}
 ObjManager* Engine::GetObjManager()
 {
	 return *reinterpret_cast<ObjManager**>(Engine::GetBaseModule()+GameClass::ObjManager);
 }
 float Engine::GetGameTime()
{
	return *(float*)(GetBaseModule() + GameClass::GameTime);
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
	return (char*)(GetBaseModule() + GameClass::BuildVersion);
}
 void Engine::PrintChat(DWORD ChatClient, const char* Massage, int Color)
{
	 auto fnPrintChat = reinterpret_cast<void(__thiscall*)(DWORD, const char*, int)>(GetBaseModule()+ Function::PrintChat);
	return fnPrintChat(ChatClient, Massage, Color);
}

 void Engine::PrintChats(int Color, const char* message, ...)
 {
	 char output[1024] = {};
	 va_list args;
	 va_start(args, message);
	 vsprintf_s(output, message, args);
	 va_end(args);

	 PrintChat(*(DWORD*)(GetBaseModule() + GameClass::ChatClient), output, Color);
 }
