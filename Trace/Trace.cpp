#include "Trace.h"
#include "DrawHook.h"
/// <summary>
/// 梦开始的地方
/// </summary>

void OnStart()
{
	//等待游戏
	while (Engine::GetGameTime() < 1.0f || !Engine::GetLocalPlayer())
		Sleep(1);
	//AllocConsole(); // 控制台
	//freopen("CONIN$", "r", stdin); //控制台
	//freopen("CONOUT$", "w", stdout);

	DrawHook::Start();
}

BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnStart, hModule, 0, 0);
		thread(OnStart).detach();
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}