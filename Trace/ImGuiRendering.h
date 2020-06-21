#pragma once
#include "Trace.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"


class ImGuiRendering
{
private:
	ImFont* _Font;
	IDirect3DDevice9* _Device;
	DWORD _D3DRS_COLORWRITEENABLE;
	ImDrawList* _DrawList;
	bool _IsSetup = false;
	ImGuiRendering() {};
	
public:
	static auto GetIns()
	{
		static auto _Draw = new ImGuiRendering();
		return _Draw;
	}

	void Setup(HWND hWnd, LPDIRECT3DDEVICE9 device);
	void Clear();
	void PreRender();
	void EndRender();
	void  DrawString(ImFont* font, float x, float y, ImU32 color, const char* message, ...);
	void  DrawString(float x, float y, ImU32 color, const char* message, ...);
	void  DrawBox(float x, float y, float w, float h, ImU32 clr, float width);
	void  DrawBox(ImVec4 scr, ImU32 clr, float width);
	void  DrawBox(Vector leftUpCorn, Vector rightDownCorn, ImU32 clr, float width);
	void  DrawLine(float x1, float y1, float x2, float y2, ImU32 clr, float thickness);
	void  DrawCircle(float x, float y, float rad, ImU32 clr, float thickness);
	void  DrawBlod(float x, float y, float w, float Blod, ImU32 clr);
	void  DrawCircle3D(Vector vPos, float flPoints, float flRadius, ImColor clrColor, float flThickness = 2.f);



};

