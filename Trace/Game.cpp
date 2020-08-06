#include "Game.h"

vector<Hero*> Game::HeroCache;
vector<Missile*> Game::MissileCache;
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
