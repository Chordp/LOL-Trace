#include "Game.h"
#include "stb_image.h"
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
					Draw->DrawString(Draw->Font16F,start.X, start.Y, IM_COL32(255, 255, 255, 255), u8"[%s]%0.fÃ×", hero->GetTitle().c_str(), length);
				}
			}
		}
	}
}
map<string, LPDIRECT3DTEXTURE9> LoadTexture()
{
	map<string, LPDIRECT3DTEXTURE9> Texture;
	
	for (auto image : Config::GetIns()->Contrast.Summoner)
	{
		LPDIRECT3DTEXTURE9 m_pTexture;
		BYTE* Image_data = new BYTE[image.second.size()];
		std::copy(image.second.begin(), image.second.end(), Image_data);
		int width, height, channel;
		auto data = stbi_load_from_memory(Image_data, image.second.size(), &width, &height, &channel, 3);

		Draw->_Device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT,&m_pTexture,nullptr);
		D3DLOCKED_RECT lock_rect;
		unsigned char* buffer = new unsigned char[width * height * (channel + 1)];
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				buffer[(i * width + j) * 4 + 0] = data[(i * width + j) * 3 + 2];
				buffer[(i * width + j) * 4 + 1] = data[(i * width + j) * 3 + 1];
				buffer[(i * width + j) * 4 + 2] = data[(i * width + j) * 3 + 0];
				buffer[(i * width + j) * 4 + 3] = 0xff;
			}
		}
		m_pTexture->LockRect(0, &lock_rect, NULL, 0);
		for (int y = 0; y < height; y++)
			memcpy((unsigned char*)lock_rect.pBits + lock_rect.Pitch * y, buffer + (width * (channel + 1)) * y, (width * (channel + 1)));
		m_pTexture->UnlockRect(0);

		Texture[image.first] = m_pTexture;
		delete[] buffer;
		delete[] Image_data;


	}
	return Texture;
}
void Game::DrawCD()
{
	static map<string, LPDIRECT3DTEXTURE9> Texture = LoadTexture();
	for (auto hero : HeroCache)
	{

		//if (hero->GetTeam() == Me->GetTeam())
		//	continue;
		////if (hero->IsAlive())
		{
			int i = 0;
			auto Screen = Vector();
			//if (Engine::WorldToScreens(hero->GetPos(), Screen))
			if(hero->GetHpBarPosition(Screen))
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
							Draw->DrawString(Draw->Font16F ,pos.x + 10, pos.y + 5, Color::White, "%0.0f", Cooldown);
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
					if (Texture.count(SpellName) != 0)
					{
						DrawList->AddImage(Texture[SpellName], pos, ImVec2(pos.x + size, pos.y + size));
						auto Cooldown = Spell->GetCooldown();
						if (!Spell->IsReady())
						{
							DrawList->AddRectFilled(pos, ImVec2(pos.x + size, pos.y + size), IM_COL32(238, 97, 109, 100));
							Draw->DrawString(Draw->Font16F, pos.x + size + 2, pos.y, Color::White, "%0.0f", Cooldown);
						}
					}
					pos.y += size;
					
				}
				
				//Draw->DrawString(Draw->Font16F, pos.x, pos.y + 5, Color::White, "%0.0f %0.0f", d, f);
				
				//cout << hero->GetSpellBook()->GetSpellSlotByID(4)->GetSpellInfo()->GetSpellName() << endl;
				//cout << hero->GetSpellBook()->GetSpellSlotByID(4)->GetSpellInfo()->GetSpellData()->GetSpellName() << endl;
				
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
					
					Draw->DrawLine(start.X, start.Y, end.X, end.Y,Color::ëÙÖ¬ºì ,4);
					Draw->DrawString(Draw->Font16F,end.X, end.Y, IM_COL32(255,255,255,255), "[%s]", hero->GetTitle().c_str());
				}
			}
		}
	}
}

void Game::DrawMissile()
{
	for (auto x : MissileCache)
	{	//auto x = p->GetSpellCastInfo();

		//if (x->IsBasicAttack() || x->IsAutoAttack())
		//	continue;
		//if (Me->GetTeam() == x->GetTeam())
		//	return;


		auto color = IM_COL32(200, 200, 200, 200);

		Vector x2, y2;
		Vector x1 = x->GetPos();
		Vector y1 = x->GetSpellEndPos();
		//Vector SPos;
		float N = x->GetSpellWidth() / 2.f;
		Engine::WorldToScreen(&x1, &x2);
		Engine::WorldToScreen(&y1, &y2);


		float dx = x2.X - y2.X;
		float dy = x2.Y - y2.Y;
		float dist = sqrt(dx * dx + dy * dy);
		dx /= dist;
		dy /= dist;


		ImVec2 startLeft = ImVec2(x2.X + N * dy, x2.Y - N * dx);
		ImVec2 startRight = ImVec2(x2.X - N * dy, x2.Y + N * dx);

		ImVec2 endLeft = ImVec2(y2.X + N * dy, y2.Y - N * dx);
		ImVec2 endRight = ImVec2(y2.X - N * dy, y2.Y + N * dx);

		ImGui::GetOverlayDrawList()->AddCircle(startLeft,10 , color,12,1);
		ImGui::GetOverlayDrawList()->AddLine(startLeft, startRight, color, 1);
		ImGui::GetOverlayDrawList()->AddCircle(startRight, 10, color, 12, 1);
		ImGui::GetOverlayDrawList()->AddLine(endLeft, endRight, color, 1);
		ImGui::GetOverlayDrawList()->AddCircle(endLeft, 10, color, 12, 1);
		ImGui::GetOverlayDrawList()->AddLine(startLeft, endLeft, color, 1);
		ImGui::GetOverlayDrawList()->AddCircle(endRight, 10, color, 12, 1);
		ImGui::GetOverlayDrawList()->AddLine(startRight, endRight, color, 1);

	}


}
