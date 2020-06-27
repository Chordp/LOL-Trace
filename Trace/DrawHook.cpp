#include "DrawHook.h"
#include "SmartHook.hpp"
#include "Menu.hpp"
#include "Hero.hpp"
#define Draw ImGuiRendering::GetIns()
#define pMenu Meun::GetIns()
#define Me Engine::GetLocalPlayer()
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace DrawHook
{
	
	MakeHook<HRESULT, LPDIRECT3DDEVICE9,RECT*, RECT*, HWND, RGNDATA* > Present;
	MakeHook<HRESULT, LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*> Reset;
	bool Init = false;
	auto oWndProc = WNDPROC();

	DWORD FindDevice(DWORD Len)
	{
		DWORD dwObjBase = 0;

		dwObjBase = (DWORD)LoadLibrary("d3d9.dll");
		while (dwObjBase++ < dwObjBase + Len)
		{
			if ((*(WORD*)(dwObjBase + 0x00)) == 0x06C7
				&& (*(WORD*)(dwObjBase + 0x06)) == 0x8689
				&& (*(WORD*)(dwObjBase + 0x0C)) == 0x8689
				) {
				dwObjBase += 2; break;
			}
		}
		return(dwObjBase);
	}
	DWORD GetDeviceAddress(int VTableIndex)
	{
		PDWORD VTable;
		*(DWORD*)&VTable = *(DWORD*)FindDevice(0x128000);
		return VTable[VTableIndex];
	}

	LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_HOME: {
				pMenu->Open = !pMenu->Open;
				break;
			}
			default:break;
			}
			break;
		}

		default:
			break;
		}

		if (pMenu->Open)
		{
			ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		}
		return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
	}

	void Start()
	{
		
		Present.Apply(GetDeviceAddress(17), Inline, [](
			LPDIRECT3DDEVICE9 _this,
			RECT* pSourceRect,
			RECT* pDestRect,
			HWND hDestWindowOverride,
			RGNDATA* pDirtyRegion)->auto
		{
			if (!Init)
			{

				oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Engine::GetWindow(), GWLP_WNDPROC, reinterpret_cast<ULONG_PTR>(WndProc)));
				Draw->Setup(Engine::GetWindow(), _this);


				auto Skin = Config::GetIns()->Setting.HeroOption[Me->GetChampionName()].Skin;
				Me->SetCharacter(Skin);
				Init = true;
			}

			Draw->PreRender();

			pMenu->Show();
			//for ( auto hero : Engine::GetObjManager()->GetAllHero() )
			//{
			//	Draw->DrawCircle3D(hero->GetPos(), 80, hero->GetAttackRange() + hero->GetBoundingRadius(), Color::White);
			//}
			auto q = Me->GetSpellBook()->GetSpellSlotByID(0)->GetCooldown();
			Engine::PrintChats(Color::Green, "%f", q);

			Draw->EndRender();




			return Present.CallOriginal(_this, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		});

		Reset.Apply(GetDeviceAddress(16), Inline, []( LPDIRECT3DDEVICE9 _this, D3DPRESENT_PARAMETERS* pp)->auto { if (Init) { ImGui_ImplDX9_InvalidateDeviceObjects(); auto hRet = Reset.CallOriginal(_this, pp); ImGui_ImplDX9_CreateDeviceObjects(); return hRet; } return Reset.CallOriginal(_this, pp); }); 
	}

}
