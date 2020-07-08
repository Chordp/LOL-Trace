#pragma once
#include "Trace.h"
#define ObjManager Engine::GetObjManager()
#include "Hero.hpp"
#include "Missile.h"

class Game
{
private:
	
	Game() {};
public:
	vector<Hero*> HeroCache;
	vector<Missile*> MissileCache;
	void GetCache()
	{
		HeroCache.clear();
		MissileCache.clear();
		auto Obj = GameObject::GetFirst();
		while (Obj)
		{
			switch (Obj->GetType())
			{
			case ObjectType::CObject:
				break;
			case ObjectType::NeutralCamp:
				break;
			case ObjectType::DeadObject:
				break;
			case ObjectType::InvalidObject:
				break;
			case ObjectType::AIBaseCommon:
				break;
			case ObjectType::AttackableUnit:
				break;
			case ObjectType::AI:
				break;
			case ObjectType::Minion:
				break;
			case ObjectType::Troy:
				break;
			case ObjectType::Dragon:
				break;
			case ObjectType::Baron:
				break;
			case ObjectType::Hero:
				HeroCache.push_back((Hero*)Obj);
				break;
			case ObjectType::Turret:
				break;
			case ObjectType::Missile:
				MissileCache.push_back((Missile*)Obj);
				break;
			case ObjectType::Building:
				break;
			case ObjectType::Nexus:
				break;
			default:
				break;
			}
			Obj = Obj->GetNext();
		}
	}
	static auto GetIns()
	{
		static auto _Game = new Game();
		return _Game;
	}
	void GankTips();
	void DrawCD();
	void DrawPath();

	void DrawMissile();
};

