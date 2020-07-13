#pragma once
#include "Trace.h"
#include "ObjManager.hpp"
class LocalPlayer;
class ObjManager;
class Engine
{
private:
	
	
public:
	/// <summary>
	/// 获取游戏模块的基地址
	/// </summary>
	/// <returns>DWORD_PTR</returns>
	static DWORD_PTR GetBaseModule();
	/// <summary>
	/// 获取游戏窗口句柄
	/// </summary>
	/// <returns>HWND</returns>
	static HWND GetWindow();
	/// <summary>
	/// 获取游戏本人的Hero对象
	/// </summary>
	/// <returns>Hero</returns>
	static LocalPlayer* GetLocalPlayer();
	/// <summary>
	/// 获取游戏对象管理
	/// </summary>
	/// <returns></returns>
	static ObjManager* GetObjManager();
	/// <summary>
	/// 获取游戏编译版本
	/// </summary>
	/// <returns></returns>
	static inline char* GetBuildVersion();
	/// <summary>
	/// 获取游戏时间 以游戏开始为准 以秒计时
	/// </summary>
	/// <returns></returns>
	static float GetGameTime();
	/// <summary>
	/// 世界坐标转屏幕坐标
	/// </summary>
	/// <param name="World"></param>
	/// <param name="Screen"></param>
	/// <returns></returns>
	static bool WorldToScreen(Vector* World, Vector* Screen);
	/// <summary>
	/// 世界坐标转屏幕坐标
	/// </summary>
	/// <param name="World"></param>
	/// <param name="Screen"></param>
	/// <returns></returns>
	static bool WorldToScreen(Vector& World, Vector& Screen);
	/// <summary>
	/// 世界坐标转屏幕坐标
	/// </summary>
	/// <param name="World"></param>
	/// <param name="Screen"></param>
	/// <returns></returns>
	static bool WorldToScreens(Vector World, Vector& Screen);
	/// <summary>
	/// 游戏聊天窗口 打印字体
	/// </summary>
	/// <param name="ChatClient"></param>
	/// <param name="Massage"></param>
	/// <param name="Color"></param>
	static void PrintChat(DWORD ChatClient, const char* Massage, int Color);
	/// <summary>
	/// 游戏聊天窗口 打印字体
	/// </summary>
	/// <param name="Color"></param>
	/// <param name="message"></param>
	/// <param name=""></param>
	static void PrintChats(int Color, const char* message, ...);

	/// <summary>
	/// 获取鼠标位置
	/// </summary>
	/// <returns></returns>
	static Vector GetMouseWorldPosition();
	/// <summary>
	/// 获取游戏延迟
	/// </summary>
	/// <returns></returns>
	static int GetPing();
};

