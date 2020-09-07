#include "Engine.hpp"

DWORD_PTR Engine::GetBaseModule()
{
	static auto Base = reinterpret_cast<DWORD_PTR> (GetModuleHandleA(nullptr));
	return Base;
}

HWND Engine::GetWindow()
{
	static auto hWnd = FindWindowA(0, "League of Legends (TM) Client");

	return hWnd;
}

LocalPlayer* Engine::GetLocalPlayer()
{
	return *reinterpret_cast<LocalPlayer**>(GetBaseModule() + GameClass::LocalPlayer);
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

 bool Engine::WorldToScreens(Vector World, Vector& Screen)
 {
	 return  Engine::WorldToScreen(&World, &Screen);
 }

char* Engine::GetBuildVersion()
{
	//return (char*)(GetBaseModule() + GameClass::BuildVersion);
}
 void Engine::PrintChat(DWORD ChatClient, const char* Massage, int Color)
{
	static auto fnPrintChat = reinterpret_cast<void(__thiscall*)(DWORD, const char*, int)>(GetBaseModule()+ Function::PrintChat);
	return fnPrintChat(ChatClient, Massage, Color);
}
 void Engine::SendChat(DWORD ChatClient, const char* Massage, int Color)
 {
	 static auto fnSendChat = reinterpret_cast<void(__thiscall*)(DWORD, const char*, int)>(GetBaseModule() + Function::SendChat);
	 return fnSendChat(ChatClient, Massage, Color);
 }
 void Engine::PrintChats(const char* message, ...)
 {
	 char output[1024] = {};
	 va_list args;
	 va_start(args, message);
	 vsprintf_s(output, message, args);
	 va_end(args);

	 PrintChat(*(DWORD*)(GetBaseModule() + GameClass::ChatClient), output,  Color::White);
 }
 void Engine::SendChats(const char* message, ...)
 {
	 char output[1024] = {};
	 va_list args;
	 va_start(args, message);
	 vsprintf_s(output, message, args);
	 va_end(args);
	 SendChat(*(DWORD*)(GetBaseModule() + GameClass::MenuGuiIns), output, 1);
 }
Vector Engine::GetMouseWorldPosition() {
	 DWORD MousePtr = GetBaseModule() + GameClass::HudInstance;
	 auto aux1 = *(DWORD*)MousePtr;
	 aux1 += 0x14;
	 auto aux2 = *(DWORD*)aux1;
	 aux2 += 0x1C;

	 float X = *(float*)(aux2 + 0x0);
	 float Y = *(float*)(aux2 + 0x4);
	 float Z = *(float*)(aux2 + 0x8);

	 return Vector{ X, Y, Z };

 }

int Engine::GetPing() {
	static auto fnGetPing = reinterpret_cast<int(__thiscall*)(PVOID)>(GetBaseModule() + Function::GetPing);
	return fnGetPing(*reinterpret_cast<PVOID*>(GetBaseModule() + GameClass::PingInstance));
}
