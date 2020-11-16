#include "Trace.h"
#include "Evader.h"
#include "Prediction.h"
#define prediction Prediction::GetReference()
bool isLeft(Vector a, Vector b, Vector c) {
	return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) > 0;
}

Vector clamp_norm_2d(Vector v, float n_max) {
	float vx = v.x, vy = v.y, vz = v.z;
	float n = sqrt(pow(vx, 2.f) + pow(vz, 2.f));
	float f = min(n, n_max) / n;
	return Vector(f * vx, vy, f * vz);
}
 void Evader::Preset()
{

	for (auto x : Game::MissileCache)
	{

		auto color = IM_COL32(200, 200, 200, 200);

		Vector x2, y2;
		Vector x1 = x->GetSpellStartPos();
		Vector y1 = x->GetSpellEndPos();
		//Vector SPos;
		float N = x->GetSpellWidth() / 2.f;
		auto localObjPos = Engine::GetLocalPlayer()->GetPos();
		Vector localObjPos_w2s;
		Engine::WorldToScreen(&localObjPos, &localObjPos_w2s);
		Engine::WorldToScreen(&x1, &x2);
		Engine::WorldToScreen(&y1, &y2);
		auto br = Engine::GetLocalPlayer()->GetBoundingRadius() * 2.f;

		if (prediction.PointOnLineSegment(
			Vector2((const float*)&x2),
			Vector2((const float*)&y2),
			Vector2((const float*)&localObjPos_w2s)), br)
		{
			Vector direction = y1 - x1;

			std::vector<Vector> points;

			Vector pos3 = y1 + Vector(direction.z * -1.0f, direction.y, direction.x * 1.0f);
			Vector pos4 = y1 + Vector(direction.z * 1.0f, direction.y, direction.x * -1.0f);

			Vector direction2 = pos3 - pos4;

			direction2 = clamp_norm_2d(direction2, br);

			Vector direction3;
			direction3.x = -direction2.x;
			direction3.y = -direction2.y;
			direction3.z = -direction2.z;
			auto bIsLeft = isLeft(x2, y2, localObjPos_w2s);
			Vector direction4 = bIsLeft ? direction3 : direction2;

			Vector evadePos = localObjPos + direction4;
		}





		float dx = x2.x - y2.x;
		float dy = x2.y - y2.y;
		float dist = sqrt(dx * dx + dy * dy);
		dx /= dist;
		dy /= dist;


		ImVec2 startLeft = ImVec2(x2.x + N * dy, x2.y - N * dx);
		ImVec2 startRight = ImVec2(x2.x - N * dy, x2.y + N * dx);

		ImVec2 endLeft = ImVec2(y2.x + N * dy, y2.y - N * dx);
		ImVec2 endRight = ImVec2(y2.x - N * dy, y2.y + N * dx);

		ImGui::GetOverlayDrawList()->AddCircle(startLeft, 10, color, 12, 1);
		ImGui::GetOverlayDrawList()->AddLine(startLeft, startRight, color, 1);
		ImGui::GetOverlayDrawList()->AddCircle(startRight, 10, color, 12, 1);
		ImGui::GetOverlayDrawList()->AddLine(endLeft, endRight, color, 1);
		ImGui::GetOverlayDrawList()->AddCircle(endLeft, 10, color, 12, 1);
		ImGui::GetOverlayDrawList()->AddLine(startLeft, endLeft, color, 1);
		ImGui::GetOverlayDrawList()->AddCircle(endRight, 10, color, 12, 1);
		ImGui::GetOverlayDrawList()->AddLine(startRight, endRight, color, 1);

	}
}
