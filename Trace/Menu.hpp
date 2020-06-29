#pragma once
#include "Trace.h"

class Meun
{
public :
	bool Open = false;
public:
	static auto GetIns()
	{
		static auto _Meun = new Meun();
		return _Meun;
	}
	void Show()
	{
		if (!Open) return;
		auto& setting = Config::GetIns()->Setting;
		auto &HeroOp  = setting.HeroOption[Engine::GetLocalPlayer()->GetChampionName()];
		ImGui::Begin(u8"Éµ±ÆÖúÊÖ");
		if (ImGui::SliderInt(u8"Æ¤·ô", &HeroOp.Skin, 0, 100))
		{
			Engine::GetLocalPlayer()->SetCharacter(HeroOp.Skin);
			Config::GetIns()->Save();
		}
		ImGui::Checkbox(u8"Â·¾¶Ô¤²â", &setting.Path); ImGui::SameLine();
		ImGui::Checkbox(u8"ÏÔÊ¾ÀäÈ´", &setting.DrawCd); ImGui::SameLine();
		ImGui::Checkbox(u8"GankÌáÊ¾", &setting.Gank);
		ImGui::End();
		
	}

};