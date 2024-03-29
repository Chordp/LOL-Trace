#include "DrawHook.h"
#include "SmartHook.hpp"
#include "Hero.hpp"
#include "Orbwalker.h"
#include "Skin.h"
#include "AIO.h"
#include "Menu.h"
#define setting Config::GetIns()->Setting
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
		EventHandle<EventType::OnWndProc>::GetIns()->Invoke<HWND, UINT, WPARAM, LPARAM>(hwnd, uMsg, wParam, lParam);
		switch (uMsg)
		{
		case WM_KEYDOWN:
		{
			
			switch (wParam)
			{
			case VK_HOME: {
				Menu::GetIns()->Open = !Menu::GetIns()->Open;
				break;
			}
			case VK_END:
			{
				for (size_t i = 0; i < 10; i++)
				{
					//Engine::SendChats(u8"/all ����?");
					//Engine::PrintChats("����?");
				}
				
				//thread([]() { for (size_t i = 0; i < 50; i++)
				//{
				//	Engine::SendChats("/all ?");
				//	Sleep(50);
				//}}).detach();
				break;
			}
			default:break;
			}
			break;
		}
		default:
			break;
		}

		if (Menu::GetIns()->Open)
		{
			ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		}
		return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
	}


	void init()
	{
		AIO::GetIns();
		Skin::GetIns();
		//Orbwalker::GetIns();
		EventHandle<EventType::OnWndProc>::GetIns()->Add(Skin::WndProc);
		EventHandle<EventType::OnPresent>::GetIns()->Add(AIO::Present);
		//EventHandle<EventType::OnPresent>::GetIns()->Add(Orbwalker::Present);

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
				init();
				oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Engine::GetWindow(), GWLP_WNDPROC, reinterpret_cast<ULONG_PTR>(WndProc)));
				Draw->Setup(Engine::GetWindow(), _this);
				
				Init = true;
			}

			Draw->PreRender();

			Game::GetIns()->GetCache();
			EventHandle<EventType::OnPresent>::GetIns()->Invoke();



			Menu::GetIns()->Show();

			Draw->EndRender();



			return Present.CallOriginal(_this, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		});

		Reset.Apply(GetDeviceAddress(16), Inline, []( LPDIRECT3DDEVICE9 _this, D3DPRESENT_PARAMETERS* pp)->auto 
		{
			if (Init) {
				Draw->Clear();
				ImGui_ImplDX9_InvalidateDeviceObjects(); 
		
				auto hRet = Reset.CallOriginal(_this, pp);
				ImGui_ImplDX9_CreateDeviceObjects();
				return hRet; 
			} 
			return Reset.CallOriginal(_this, pp);
		}); 
	}

}
