#pragma once
#include "Trace.h"

class Meun
{
public :
	bool Open = true;
public:
	static auto GetIns()
	{
		static auto _Meun = new Meun();
		return _Meun;
	}
	void Show()
	{
		if (!Open) return;
		auto &HeroOp  = Config::GetIns()->Setting.HeroOption[Engine::GetLocalPlayer()->GetChampionName()];
		ImGui::Begin(u8"ɵ������");
		if (ImGui::SliderInt(u8"Ƥ��", &HeroOp.Skin, 0, 100))
		{
			Engine::GetLocalPlayer()->SetCharacter(HeroOp.Skin);
			Config::GetIns()->Save();
		}
		ImGui::End();
		
	}

};