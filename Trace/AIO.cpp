#include "AIO.h"
#include "Hero.hpp"
#include "Game.h"
namespace St
{

}
AIO::AIO()
{
	Menu::GetIns()->AddMenu(u8"意识") =
	{
		Menu::Checkbox(u8"绘制CD"),
		Menu::AddMenu(u8"路径预测",
		{
			Menu::Checkbox(u8"开启"),
			Menu::SliderInt(u8"线宽",1,10),
			Menu::ColorEdit4(u8"颜色")
		}),
		Menu::AddMenu(u8"Gank提示",
		{
			Menu::Checkbox(u8"开启"),
			Menu::SliderInt(u8"线宽",1,50),
			Menu::SliderInt(u8"距离",1000,10000),
			Menu::ColorEdit4(u8"颜色")
		})
	};
	auto& Config = Menu::GetIns()->config[u8"意识"];
	try
	{
		auto& jGank = Config[u8"Gank提示"];
		auto& jPath = Config[u8"路径预测"];
		tuple <bool, int, int, int>  Gank(
			jGank[u8"开启"].get<bool>(),
			jGank[u8"线宽"].get<int>(),
			jGank[u8"距离"].get<int>(),
			jGank[u8"颜色"].get<ImU32>());
		tuple <bool, int, int> Path(
			jPath[u8"开启"].get<bool>(),
			jPath[u8"线宽"].get<int>(),
			jPath[u8"颜色"].get<ImU32>());
	}
	catch (const std::exception&)
	{
		Config[u8"绘制CD"] = true;
		Config[u8"路径预测"] =
		{
			{u8"开启",true},
			{u8"线宽",4},
			{u8"颜色",0x980F48D7}
		};


		Config[u8"Gank提示"] =
		{
			{u8"开启",true},
			{u8"线宽",3},
			{u8"距离",3000},
			{u8"颜色",0xA100FF2E}
		};

			
	};
}
	


void AIO::GankTips(tuple <bool, int, int, ImU32> & v)
{
	for (auto hero : Game::HeroCache)
	{
		if (hero->GetTeam() != Me->GetTeam())
		{
			auto LocalPos = Me->GetPos();
			auto TargetPos = hero->GetPos();
			auto length = (TargetPos - LocalPos).length();
			if (length < get<2>(v))
			{
				Vector start, end;
				if (!Engine::WorldToScreen(&TargetPos, &end))
				{
					Engine::WorldToScreen(&LocalPos, &start);

					Draw->DrawLine(start.X, start.Y, end.X, end.Y, get<3>(v), get<1>(v));
					Draw->DrawString(Draw->Font16F, start.X, start.Y, Color::White, u8"[%s]%0.f米", hero->GetTitle().c_str(), length);
				}
			}
		}
	}
}
void AIO::DrawCD()
{

	for (auto hero : Game::HeroCache)
	{
		if (hero == Me)
			continue;
		//if (hero->GetTeam() == Me->GetTeam())
		//	continue;
		////if (hero->IsAlive())
		{
			int i = 0;
			auto Screen = Vector();

			if (hero->GetHpBarPosition(Screen))
			{
				auto DrawList = ImGui::GetOverlayDrawList();
				ImVec2 pos = ImVec2(Screen.X - 44, Screen.Y - 3);
				const float width = 26;
				float height = pos.y + 5;

				DrawList->AddRectFilled(pos, ImVec2(pos.x + 107, height), IM_COL32(0, 0, 0, 200));
				for (size_t i = 0; i < 4; i++)
				{
					auto Spell = hero->GetSpellBook()->GetSpellSlotByID((SpellSlotID)i);
					auto SpellState = (int)hero->GetSpellBook()->GetSpellState((SpellSlotID)i);
					auto IsReady = Spell->IsReady() ;
				

					if (IsReady)
					{
						DrawList->AddRectFilled(pos, ImVec2(pos.x + width, height), IM_COL32(18, 170, 156, 200)); // q
					}
					else if ((SpellState & (int)SpellState::Cooldown) != 0)
					{
						auto Cooldown = Spell->GetCooldown();
						float percent = 1 - Cooldown / Spell->GetCD();
						DrawList->AddRectFilled(pos, ImVec2(pos.x + width * percent, height), IM_COL32(238, 72, 99, 200)); // q
						Draw->DrawString(Draw->Font16F, pos.x + 10, pos.y + 5, Color::White, "%0.0f", Cooldown);	
					}
					else if ((SpellState & (int)SpellState::NoMana) != 0)
					{

						DrawList->AddRectFilled(pos, ImVec2(pos.x + width, height), IM_COL32(82, 82, 136, 200)); // q
					}
					else
					{
						
						DrawList->AddRectFilled(pos, ImVec2(pos.x + width, height), IM_COL32(238, 72, 99, 200)); // q
					}

					pos.x += width + 1;

				}
				pos.y -= 24;
				const int size = 15;
				for (size_t i = 4; i < 6; i++)
				{
					auto Spell = hero->GetSpellBook()->GetSpellSlotByID((SpellSlotID)i);
					auto SpellName = Spell->GetSpellInfo()->GetSpellName();

					auto Texture = Draw->GetTexture(SpellName);

					auto Cooldown = Spell->GetCooldown();
					 

					if (Texture != nullptr)
					{
						DrawList->AddImage(Texture, pos, ImVec2(pos.x + size, pos.y + size));

						if (!Spell->IsReady())
						{
							DrawList->AddRectFilled(pos, ImVec2(pos.x + size, pos.y + size), IM_COL32(238, 97, 109, 100));
							Draw->DrawString(Draw->Font16F, pos.x + size + 2, pos.y, Color::White, "%0.0f", Cooldown);
						}
					}
					else
					{
						Draw->DrawString(Draw->Font16F, pos.x, pos.y, Color::White, "%0.0f", Cooldown);
					}
					pos.y += size;

				}


			}
		}
	}
}
void AIO::DrawPath(tuple <bool, int, ImU32 >& v)
{
	for (auto hero : Game::HeroCache)
	{
		if (hero->GetTeam() == Me->GetTeam())
			continue;

		AIManager* AiMGR = hero->GetAIManager();

		if (AiMGR && AiMGR->IsMoving())
		{
			auto path = AiMGR->GetPathList();
			if (!path.empty())
			{
				Vector start, end;
				if (Engine::WorldToScreens(hero->GetPos(), start) &&
					Engine::WorldToScreens(path.back(), end))
				{

					Draw->DrawLine(start.X, start.Y, end.X, end.Y, get<2>(v), get<1>(v));
					Draw->DrawString(Draw->Font16F, end.X, end.Y, Color::White, "[%s]", hero->GetTitle().c_str());
				}
			}
		}
	}
}




void AIO::Present()
{
	auto& Config = Menu::GetIns()->config[u8"意识"];
	try
	{
		
		auto& jGank = Config[u8"Gank提示"];
		auto& jPath = Config[u8"路径预测"];
		tuple <bool, int, int, ImU32>  Gank(
			jGank[u8"开启"].get<bool>(),
			jGank[u8"线宽"].get<int>(),
			jGank[u8"距离"].get<int>(),
			jGank[u8"颜色"].get<ImU32>());
		tuple <bool, int, ImU32> Path(
			jPath[u8"开启"].get<bool>(),
			jPath[u8"线宽"].get<int>(),
			jPath[u8"颜色"].get<ImU32>());



		bool CoolDown = Config[u8"绘制CD"];
		if(CoolDown) AIO::GetIns()->DrawCD();
		if(get<0>(Path))AIO::GetIns()->DrawPath(Path);
		if (get<0>(Gank))AIO::GetIns()->GankTips(Gank);


	}
	catch (const std::exception&)
	{

	}


}


