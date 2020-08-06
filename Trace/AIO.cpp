#include "AIO.h"
#include "Hero.hpp"
#include "Game.h"
namespace St
{

}
AIO::AIO()
{
	Menu::GetIns()->AddMenu(u8"��ʶ") =
	{
		Menu::Checkbox(u8"����CD"),
		Menu::AddMenu(u8"·��Ԥ��",
		{
			Menu::Checkbox(u8"����"),
			Menu::SliderFloat(u8"�߿�",1,10),
			Menu::ColorEdit4(u8"��ɫ")
		}),
		Menu::AddMenu(u8"Gank��ʾ",
		{
			Menu::Checkbox(u8"����"),
			Menu::SliderFloat(u8"�߿�",1,50),
			Menu::SliderInt(u8"����",1000,10000),
			Menu::ColorEdit4(u8"��ɫ")
		})
	};
	auto& Config = Menu::GetIns()->config[u8"��ʶ"];
	try
	{
		auto& jGank = Config[u8"Gank��ʾ"];
		auto& jPath  = Config[u8"·��Ԥ��"];
		tuple <bool, float, int, int>  Gank(jGank[u8"����"], jGank[u8"�߿�"], jGank[u8"����"], jGank[u8"��ɫ"]);
		tuple <bool, float, int> Path(jPath[u8"����"], jPath[u8"�߿�"], jPath[u8"��ɫ"]);
		bool CoolDown = Config[u8"����CD"];
	}
	catch (const std::exception&)
	{
		Config =
		{
			"u8Gank��ʾ",
			{
				{"u8����",true},
				{u8"�߿�",10},
				{u8"����",3000},
				{u8"��ɫ",Color::Red}
			},
			u8"·��Ԥ��",
			{
				{u8"����",true},
				{u8"�߿�",4},
				{u8"��ɫ",Color::Red}
			},
			{u8"����CD",true}
		};
	}
	
}

void AIO::GankTips(tuple <bool, float, int, int> & v)
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
					Draw->DrawString(Draw->Font16F, start.X, start.Y, Color::White, u8"[%s]%0.f��", hero->GetTitle().c_str(), length);
				}
			}
		}
	}
}
void AIO::DrawCD()
{
	for (auto hero : Game::HeroCache)
	{

		//if (hero->GetTeam() == Me->GetTeam())
		//	continue;
		////if (hero->IsAlive())
		{
			int i = 0;
			auto Screen = Vector();
			//if (Engine::WorldToScreens(hero->GetPos(), Screen))
			if (hero->GetHpBarPosition(Screen))
			{

				auto DrawList = ImGui::GetOverlayDrawList();
				ImVec2 pos = ImVec2(Screen.X - 44, Screen.Y - 3);
				const float width = 26;
				float height = pos.y + 5;

				DrawList->AddRectFilled(pos, ImVec2(pos.x + 107, height), IM_COL32(0, 0, 0, 200));

				for (auto Spell : hero->GetSpellBook()->GetAllSpellSlot())
				{
					auto IsReady = Spell->IsReady();
					auto Cooldown = Spell->GetCooldown();

					if (IsReady)
					{
						DrawList->AddRectFilled(pos, ImVec2(pos.x + width, height), IM_COL32(18, 170, 156, 200)); // q
					}
					else
					{
						if (Spell->GetLevel() > 0)
						{
							float percent = 1 - Cooldown / Spell->GetCD();
							DrawList->AddRectFilled(pos, ImVec2(pos.x + width * percent, height), IM_COL32(238, 72, 99, 200)); // q
							Draw->DrawString(Draw->Font16F, pos.x + 10, pos.y + 5, Color::White, "%0.0f", Cooldown);
						}
						else
							DrawList->AddRectFilled(pos, ImVec2(pos.x + width, height), IM_COL32(238, 72, 99, 200)); // q
					}

					pos.x += width + 1;

				}
				pos.y -= 24;
				const int size = 15;
				for (size_t i = 4; i < 6; i++)
				{
					auto Spell = hero->GetSpellBook()->GetSpellSlotByID(i);
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
void AIO::DrawPath(tuple <bool, float, int >& v)
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
	auto& Config = Menu::GetIns()->config[u8"��ʶ"];
	try
	{
		
		auto& jGank = Config[u8"Gank��ʾ"];
		auto& jPath = Config[u8"·��Ԥ��"];
		tuple <bool, float, int, int>  Gank(jGank[u8"����"], jGank[u8"�߿�"], jGank[u8"����"], jGank[u8"��ɫ"]);
		tuple <bool, float, int> Path(jPath[u8"����"], jPath[u8"�߿�"], jPath[u8"��ɫ"]);
		bool CoolDown = Config[u8"����CD"];
		if(CoolDown) AIO::GetIns()->DrawCD();
		if(get<0>(Path))AIO::GetIns()->DrawPath(Path);
		if (get<0>(Gank))AIO::GetIns()->GankTips(Gank);
		cout << 123 << endl;


	}
	catch (const std::exception&)
	{

	}


}


