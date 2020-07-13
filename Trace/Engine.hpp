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
	/// ��ȡ��Ϸģ��Ļ���ַ
	/// </summary>
	/// <returns>DWORD_PTR</returns>
	static DWORD_PTR GetBaseModule();
	/// <summary>
	/// ��ȡ��Ϸ���ھ��
	/// </summary>
	/// <returns>HWND</returns>
	static HWND GetWindow();
	/// <summary>
	/// ��ȡ��Ϸ���˵�Hero����
	/// </summary>
	/// <returns>Hero</returns>
	static LocalPlayer* GetLocalPlayer();
	/// <summary>
	/// ��ȡ��Ϸ�������
	/// </summary>
	/// <returns></returns>
	static ObjManager* GetObjManager();
	/// <summary>
	/// ��ȡ��Ϸ����汾
	/// </summary>
	/// <returns></returns>
	static inline char* GetBuildVersion();
	/// <summary>
	/// ��ȡ��Ϸʱ�� ����Ϸ��ʼΪ׼ �����ʱ
	/// </summary>
	/// <returns></returns>
	static float GetGameTime();
	/// <summary>
	/// ��������ת��Ļ����
	/// </summary>
	/// <param name="World"></param>
	/// <param name="Screen"></param>
	/// <returns></returns>
	static bool WorldToScreen(Vector* World, Vector* Screen);
	/// <summary>
	/// ��������ת��Ļ����
	/// </summary>
	/// <param name="World"></param>
	/// <param name="Screen"></param>
	/// <returns></returns>
	static bool WorldToScreen(Vector& World, Vector& Screen);
	/// <summary>
	/// ��������ת��Ļ����
	/// </summary>
	/// <param name="World"></param>
	/// <param name="Screen"></param>
	/// <returns></returns>
	static bool WorldToScreens(Vector World, Vector& Screen);
	/// <summary>
	/// ��Ϸ���촰�� ��ӡ����
	/// </summary>
	/// <param name="ChatClient"></param>
	/// <param name="Massage"></param>
	/// <param name="Color"></param>
	static void PrintChat(DWORD ChatClient, const char* Massage, int Color);
	/// <summary>
	/// ��Ϸ���촰�� ��ӡ����
	/// </summary>
	/// <param name="Color"></param>
	/// <param name="message"></param>
	/// <param name=""></param>
	static void PrintChats(int Color, const char* message, ...);

	/// <summary>
	/// ��ȡ���λ��
	/// </summary>
	/// <returns></returns>
	static Vector GetMouseWorldPosition();
	/// <summary>
	/// ��ȡ��Ϸ�ӳ�
	/// </summary>
	/// <returns></returns>
	static int GetPing();
};

