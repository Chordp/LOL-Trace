#include "ImGuiRendering.h"
#include "stb_image.h"
#include <filesystem>
using Path = std::filesystem::path;
void ImGuiRendering::Setup(HWND hWnd, LPDIRECT3DDEVICE9 device)
{

	if (_IsSetup) return;
	_Device = device;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(device);
	Path path("C:\\Windows\\Fonts\\msyhbd.ttc");
	if (!std::filesystem::exists(path))
	{
		path = "C:\\Windows\\Fonts\\msyhbd.ttf";
		
	}
	Font16F = io.Fonts->AddFontFromFileTTF(path.string().c_str(), 16.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
	Font14F = io.Fonts->AddFontFromFileTTF(path.string().c_str(), 14.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
	//ImGui_ImplDX9_CreateDeviceObjects();
	
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0;
	ImGui::StyleColorsLight();
	_DrawList = ImGui::GetOverlayDrawList();
	_IsSetup = true;
}
LPDIRECT3DTEXTURE9 ImGuiRendering::CreateTexture(vector<BYTE> &p)
{
	LPDIRECT3DTEXTURE9 m_pTexture;
	BYTE* Image_data = new BYTE[p.size()];
	std::copy(p.begin(), p.end(), Image_data);
	int width, height, channel;
	auto data = stbi_load_from_memory(Image_data, p.size(), &width, &height, &channel, 3);

	_Device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT, &m_pTexture, nullptr);
	D3DLOCKED_RECT lock_rect;
	unsigned char* buffer = new unsigned char[width * height * (channel + 1)];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			buffer[(i * width + j) * 4 + 0] = data[(i * width + j) * 3 + 2];
			buffer[(i * width + j) * 4 + 1] = data[(i * width + j) * 3 + 1];
			buffer[(i * width + j) * 4 + 2] = data[(i * width + j) * 3 + 0];
			buffer[(i * width + j) * 4 + 3] = 0xff;
		}
	}
	m_pTexture->LockRect(0, &lock_rect, NULL, 0);
	for (int y = 0; y < height; y++)
		memcpy((unsigned char*)lock_rect.pBits + lock_rect.Pitch * y, buffer + (width * (channel + 1)) * y, (width * (channel + 1)));
	m_pTexture->UnlockRect(0);

	delete[] buffer;
	delete[] Image_data;

	return m_pTexture;
}
LPDIRECT3DTEXTURE9 ImGuiRendering::GetTexture(string name)
{
	if (Texture.count(name) == 0)
	{
		auto& Summoner = Menu::GetIns()->Contrast.Summoner;

		if (Summoner.count(name) == 0)
			return nullptr;
		Texture[name] = CreateTexture(Summoner[name]);
		return Texture[name];
	}
	else
	{
		return Texture[name];
	}
}

void ImGuiRendering::Create()
{
	if (!_IsSetup)return;
	auto& Summoner = Menu::GetIns()->Contrast.Summoner;
	for (auto t : Texture)
	{
		if (t.second)
		{
			t.second = CreateTexture(Summoner[t.first]);
			cout << t.first << "\t" << "create" << endl;
		}
	}

}

void ImGuiRendering::Clear()
{
	if (!_IsSetup)return;

	for (auto t : Texture)
	{
		auto& p = t.second;
		if (p != nullptr)
		{
			p->Release();
			p = nullptr;
		}

	}
	Texture.clear();
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
	float flPoint = M_PI * 2.0f / flPoints;

	for (float flAngle = 0; flAngle < (M_PI * 2.0f); flAngle += flPoint)
	{
		Vector vStart(flRadius * cosf(flAngle) + vPos.X, vPos.Y, flRadius * sinf(flAngle) + vPos.Z);
		Vector vEnd(flRadius * cosf(flAngle + flPoint) + vPos.X, vPos.Y, flRadius * sinf(flAngle + flPoint) + vPos.Z);

		Vector vStartScreen, vEndScreen;
		//if (!Engine::WorldToScreen(&vStart, &vStartScreen) || !Engine::WorldToScreen(&vEnd, &vEndScreen))
		//	continue;
		Engine::WorldToScreen(&vStart, &vStartScreen); Engine::WorldToScreen(&vEnd, &vEndScreen);
		DrawLine(vStartScreen.X, vStartScreen.Y, vEndScreen.X, vEndScreen.Y, clrColor, flThickness);
	}
}