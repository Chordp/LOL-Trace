#pragma once
#include "Trace.h"

class Meun
{
public :
	bool Open = true;
	int Skin = 0;
public:
	static auto GetIns()
	{
		static auto _Meun = new Meun();
		return _Meun;
	}
	void Show()
	{
		if (!Open) return;
		
		ImGui::Begin(u8"ɵ������");
		if (ImGui::SliderInt(u8"Ƥ��", &Skin, 0, 100))
		{
			Engine::GetLocalPlayer()->SetCharacter(Skin);
		}
		ImGui::End();
		
	}

};