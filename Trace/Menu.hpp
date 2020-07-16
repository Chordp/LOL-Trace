#pragma once
#include "Trace.h"
#include "Hero.hpp"
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
		ImGui::Begin(u8"…µ±∆÷˙ ÷");
		if (ImGui::SliderInt(u8"∆§∑Ù", &HeroOp.Skin, 0, 100))
		{
			Engine::GetLocalPlayer()->SetCharacter(HeroOp.Skin);
			Config::GetIns()->Save();
		}
		ImGui::Checkbox(u8"¬∑æ∂‘§≤‚", &setting.Path); ImGui::SameLine();
		ImGui::Checkbox(u8"œ‘ æ¿‰»¥", &setting.DrawCd); ImGui::SameLine();
		ImGui::Checkbox(u8"GankÃ· æ", &setting.Gank); ImGui::SameLine();
		ImGui::Checkbox(u8"Ez∂„±‹", &setting.EzEvade);
		ImGui::End();
		
	}

};