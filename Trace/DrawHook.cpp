#include "DrawHook.h"
#include "SmartHook.hpp"

#define Draw ImGuiRendering::GetIns()
namespace DrawHook
{
	
	MakeHook<HREFTYPE, LPDIRECT3DDEVICE9,RECT*, RECT*, HWND, RGNDATA* > Present;
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

	bool Init = false;

	DWORD GetDeviceAddress(int VTableIndex)
	{
		PDWORD VTable;
		*(DWORD*)&VTable = *(DWORD*)FindDevice(0x128000);
		return VTable[VTableIndex];
	}
	void Start()
	{
		Present.Apply(GetDeviceAddress(17),Inline,[](
			LPDIRECT3DDEVICE9 _this,
			RECT* pSourceRect, 
			RECT* pDestRect, 
			HWND hDestWindowOverride, 
			RGNDATA* pDirtyRegion)->auto
		{
			if(!Init)
			{
				Draw->Setup(Engine::GetGameWindow(), _this);
				Init = true;
			}

			Draw->PreRender();

			ImGui::Begin("123");
			ImGui::End();
			
			Draw->EndRender();



			
			return Present.CallOriginal(_this, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		});
	}

}
