#include "Game.h"
static map<int, string> ESpellSlot = {
	{0,"Q"},
	{1,"W"},
	{2,"E"},
	{3,"R"},
	{4,"D"},
	{5,"F"}
};
void Game::GankTips()
{
	for (auto hero : HeroCache)
	{
		if(hero->GetTeam() != Me->GetTeam())
		{
			auto LocalPos = Me->GetPos();
			auto TargetPos = hero->GetPos();
			auto length = (TargetPos - LocalPos).length();
			if(length <3000)
			{ 
				Vector start, end;
				if(!Engine::WorldToScreen(&TargetPos, &end))
				{
					Engine::WorldToScreen(&LocalPos, &start);
						
					Draw->DrawLine(start.X, start.Y, end.X, end.Y, IM_COL32(255, 20, 20, 255), 10);
					Draw->DrawString(start.X, start.Y, IM_COL32(255, 255, 255, 255), u8"[%s]%0.fÃ×", hero->GetTitle().c_str(), length);
				}
			}
		}
	}
}
void Game::DrawCD()
{

	for (auto hero : HeroCache)
	{
		if (hero->GetTeam() == Me->GetTeam())
			continue;
		if (hero->IsAlive())
		{
			int i = 0;
			auto Screen = Vector();
			//if (Engine::WorldToScreens(hero->GetPos(), Screen))
			if(hero->GetHpBarPosition(Screen))
			{
				for (auto Spell : hero->GetSpellBook()->GetAllSpellSlot())
				{
					
				
					if (Spell->IsReady())
					{
						Draw->DrawString(Screen.X + (i * 28) - 84, Screen.Y, IM_COL32(21, 229, 132, 200), "[%s]", ESpellSlot[i].c_str());
						
					}
					else
					{
						if (Spell->GetLevel()>0)
							Draw->DrawString(Screen.X + (i * 28) - 84, Screen.Y, IM_COL32(234, 20, 40, 200), "[%.0f]", Spell->GetCooldown());
						else
							Draw->DrawString(Screen.X + (i * 28) - 84, Screen.Y, IM_COL32(234, 20, 40, 200), "[%s]", ESpellSlot[i].c_str());
					}
					i++;
				}
				
			}
		}
	}
}

void Game::DrawPath()
{
	for (auto hero : HeroCache)
	{
		if (hero->GetTeam() == Me->GetTeam())
			continue;
		AIManager* AiMGR = hero->GetAIManager();
		if (AiMGR && AiMGR->IsMoving())
		{
			auto path = AiMGR->GetPathList();
			if(!path.empty())
			{
				Vector start, end;
							

				if(Engine::WorldToScreens(hero->GetPos(), start)	&&
					Engine::WorldToScreens(path.back(), end))
				{ 
					
					Draw->DrawLine(start.X, start.Y, end.X, end.Y,Color::¾°Ì©À¶ ,2);
					Draw->DrawString(end.X, end.Y, IM_COL32(255,255,255,255), "[%s]", hero->GetTitle().c_str());
				}
			}
		}
	}
}
