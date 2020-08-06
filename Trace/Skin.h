#pragma once
#include "Engine.hpp"
class Skin
{
	Skin();
public:
	static auto GetIns()
	{
		static auto _skin = new Skin();
		return _skin;
	}
	static void WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

