#pragma once
#include "Trace.h"
#include "Hero.hpp"
#include "Missile.h"
#include "Game.h"


class LinePrediction
{
public:

	//struct LineInfo_t
	//{
	//	CSpellData* data;
	//};

	LinePrediction()
	{

	}


	float Length(Vector& vec)
	{
		return (float)sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	}

	void Normalize(Vector& vec)
	{
		float length = Length(vec);
		if (length != 0)
		{
			float inv = 1.0f / length;
			vec.x *= inv;
			vec.y *= inv;
			vec.z *= inv;
		}
	}

	Vector Calculate(Hero* target, float range, float missilespeed, float casttime);

	~LinePrediction()
	{
	}

private:


};

class CirclePrediction
{
public:



	float Length(Vector& vec)
	{
		return (float)sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	}

	void Normalize(Vector& vec)
	{
		float length = Length(vec);
		if (length != 0)
		{
			float inv = 1.0f / length;
			vec.x *= inv;
			vec.y *= inv;
			vec.z *= inv;
		}
	}

	Vector Calculate(Hero* target, float range, float casttime);

	CirclePrediction()
	{
	}

	~CirclePrediction()
	{
	}

private:




};
class Prediction
{
	Prediction();
public:
	static Prediction* GetInstance();
	static Prediction& GetReference();

	~Prediction();

	bool BoundBox(Vector2 cursorPos, Vector2 Pos, Vector2 Size)
	{
		return Pos.x >= cursorPos.x - Size.x && Pos.x <= cursorPos.x + Size.x && Pos.y >= cursorPos.y - Size.y && Pos.y <= cursorPos.y + Size.y  && cursorPos.x >= Pos.x && cursorPos.y >= Pos.y;
	}

	bool PointInLineSegment(Vector2 segmentStart, Vector2 segmentEnd, Vector2 point)
	{

		auto crossproduct = (point.y - segmentStart.y) * (segmentEnd.x - segmentStart.x) - (point.x - segmentStart.x) * (segmentEnd.y - segmentStart.y);
		if (abs(crossproduct) > 2)
		{
			return false;
		}

		auto dotproduct = (point.x - segmentStart.x) * (segmentEnd.x - segmentStart.x) + (point.y - segmentStart.y) * (segmentEnd.y - segmentStart.y);
		if (dotproduct < 0)
		{
			return false;
		}

		auto squaredlengthba = (segmentEnd.x - segmentStart.x) * (segmentEnd.x - segmentStart.x) + (segmentEnd.y - segmentStart.y) * (segmentEnd.y - segmentStart.y);
		if (dotproduct > squaredlengthba)
		{
			return false;
		}

		return true;
	}

	bool PointOnLineSegment(Vector2 pt1, Vector2 pt2, Vector2 pt, double epsilon = 0.001)
	{
		if (pt.x - std::fmax(pt1.x, pt2.x) > epsilon ||
			std::fmin(pt1.x, pt2.x) - pt.x > epsilon ||
			pt.y - std::fmax(pt1.y, pt2.y) > epsilon ||
			std::fmin(pt1.y, pt2.y) - pt.y > epsilon)
			return false;

		if (abs(pt2.x - pt1.x) < epsilon)
			return abs(pt1.x - pt.x) < epsilon || abs(pt2.x - pt.x) < epsilon;
		if (abs(pt2.y - pt1.y) < epsilon)
			return abs(pt1.y - pt.y) < epsilon || abs(pt2.y - pt.y) < epsilon;

		double x = pt1.x + (pt.y - pt1.y) * (pt2.x - pt1.x) / (pt2.y - pt1.y);
		double y = pt1.y + (pt.x - pt1.x) * (pt2.y - pt1.y) / (pt2.x - pt1.x);

		return abs(pt.x - x) < epsilon || abs(pt.y - y) < epsilon;
	}

	std::vector<GameObject*> getEnemyMinions() {
		std::vector<GameObject*> objets;
		for (auto obj : Game::MinionCache) {
			
				if (obj->IsAlive() && obj->IsVisible() && 
					obj->GetTeam() != Engine::GetLocalPlayer()->GetTeam() &&
					obj->IsTargetable() && obj->GetMaxHealth() < 15000.0f) {
					objets.push_back(obj);
				}
			

		}
		return objets;
	}
	enum class CollisionType
	{
		Minion
	};
	bool IsCollisioned(CollisionType type, Vector3 vec)
	{
		auto local = Engine::GetLocalPlayer();
		switch (type)
		{
		case CollisionType::Minion: {
			for (auto minion : getEnemyMinions())
			{
				auto base = (GameObject*)minion;
				auto basecord = base->GetPos();//LinePred->Calculate(base);

				Vector localPos, vecPos, minionPos;
				if (!Engine::WorldToScreen(&basecord, &minionPos))
					continue;

				auto localWorldPos = local->GetPos();
				Engine::WorldToScreen(&localWorldPos, &localPos);
				Engine::WorldToScreen(&vec, &vecPos);
#ifdef linepred
				render.DrawLine(pt.x, pt.y, pt2.x, pt2.y, 10, D3DCOLOR_ARGB(255, 255, 20, 147));
				render.DrawLine(pt2.x, pt2.y, pt1.x, pt1.y, 10, D3DCOLOR_ARGB(255, 255, 165, 0));
#endif

				if (PointOnLineSegment(
					Vector2((const float*)&localPos),
					Vector2((const float*)&vecPos),
					Vector2((const float*)&minionPos),
					static_cast<double>(base->GetBoundingRadius()) * 2.0))
				{
					return true;
				}
			}
			break;
		}
		}

		return false;
	}



	LinePrediction* LinePred;
	CirclePrediction* CircPred;
};

