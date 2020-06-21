#include "ImGuiRendering.h"

void ImGuiRendering::Setup(HWND hWnd, LPDIRECT3DDEVICE9 device)
{
	if (_IsSetup) return;
	_Device = device;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(device);

	_Font =  io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyhl.ttc", 14.f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	ImGuiStyle style = ImGui::GetStyle();
	ImGui::StyleColorsLight();
	_DrawList = ImGui::GetOverlayDrawList();
	_IsSetup = true;
	
}
void ImGuiRendering::Clear()
{
	if (!_IsSetup)return;
	
}

void ImGuiRendering::PreRender()
{
	_Device->GetRenderState(D3DRS_COLORWRITEENABLE, &_D3DRS_COLORWRITEENABLE);
	_Device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void ImGuiRendering::EndRender()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	_Device->SetRenderState(D3DRS_COLORWRITEENABLE, _D3DRS_COLORWRITEENABLE);
}

void ImGuiRendering::DrawString(ImFont* font, float x, float y, ImU32 color, const char* message, ...)
{
	
	char output[1024] = {};
	va_list args;
	va_start(args, message);
	vsprintf_s(output, message, args);
	va_end(args);
	auto coord = ImVec2(x, y);
	auto size = ImGui::CalcTextSize(output);
	auto coord_out = ImVec2{ coord.x + 1.f, coord.y + 1.f };

	_DrawList->PushTextureID(font->ContainerAtlas->TexID);
	ImGui::PushFont(font);
	_DrawList->AddText(coord, color, output);
	ImGui::PopFont();
}
void ImGuiRendering::DrawString(float x, float y, ImU32 color, const char* message, ...)
{
	char output[1024] = {};
	va_list args;
	va_start(args, message);
	vsprintf_s(output, message, args);
	va_end(args);
	auto coord = ImVec2(x, y);
	auto coord2 = ImVec2(coord.x + 1, coord.y + 1);
	auto size = ImGui::CalcTextSize(output);
	auto coord_out = ImVec2{ coord.x - 1, coord.y - 1 };
	_DrawList->AddText(coord2, IM_COL32(0, 0, 0, 200), output);
	_DrawList->AddText(coord_out, IM_COL32(0, 0, 0, 200), output);
	_DrawList->AddText(coord, color, output);

}
void ImGuiRendering::DrawBox(float x, float y, float w, float h, ImU32 clr, float width)
{
	ImVec2 min = ImVec2(x, y);
	ImVec2 max = ImVec2(w, h);
	_DrawList->AddRect(min, max, clr, 0.0F, -1, width);
}
void ImGuiRendering::DrawBox(ImVec4 scr, ImU32 clr, float width)
{
	ImVec2 min = ImVec2(scr.x, scr.y);
	ImVec2 max = ImVec2(scr.z, scr.w);
	_DrawList->AddRect(min, max, clr, 0.0F, -1, width);
}
void ImGuiRendering::DrawBox(Vector leftUpCorn, Vector rightDownCorn, ImU32 clr, float width)
{
	ImVec2 min = ImVec2(leftUpCorn.X, leftUpCorn.Y);
	ImVec2 max = ImVec2(rightDownCorn.X, rightDownCorn.Y);
	_DrawList->AddRect(min, max, clr, 0.0F, -1, width);
}
void ImGuiRendering::DrawLine(float x1, float y1, float x2, float y2, ImU32 clr, float thickness = 1.0f)
{
	_DrawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), clr, thickness);

}
void ImGuiRendering::DrawCircle(float x, float y, float rad, ImU32 clr, float thickness)
{
	int line = rad / 2;
	_DrawList->AddCircle(ImVec2(x, y), rad, clr, line, thickness);
}
void ImGuiRendering::DrawBlod(float x, float y, float w, float Blod, ImU32 clr)
{
	_DrawList->AddRectFilled(ImVec2(x - 1, y - 1), ImVec2(x + w + 2, y + 5 + 2), ImColor(IM_COL32(0, 0, 0, 255)));
	_DrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + 5), ImColor(IM_COL32(255, 255, 255, 255)));
	_DrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + w * Blod / 100.0f, y + 5), clr);
}
void ImGuiRendering::DrawCircle3D(Vector vPos, float flPoints, float flRadius, ImColor clrColor, float flThickness)
{
	//float flPoint = D3DX_PI * 2.0f / flPoints;

	//for (float flAngle = 0; flAngle < (D3DX_PI * 2.0f); flAngle += flPoint)
	//{
	//	Vector vStart(flRadius * cosf(flAngle) + vPos.X, vPos.Y, flRadius * sinf(flAngle) + vPos.Z);
	//	Vector vEnd(flRadius * cosf(flAngle + flPoint) + vPos.X, vPos.Y, flRadius * sinf(flAngle + flPoint) + vPos.Z);

	//	Vector vStartScreen, vEndScreen;
	//	if (!Functions.WorldToScreen(&vStart, &vStartScreen) || !Functions.WorldToScreen(&vEnd, &vEndScreen))
	//		continue;

	//	DrawLine(vStartScreen.X, vStartScreen.Y, vEndScreen.X, vEndScreen.Y, clrColor, flThickness);
	//}
}
