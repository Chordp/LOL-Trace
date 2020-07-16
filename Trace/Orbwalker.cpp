#include "Orbwalker.h"
#include "Hero.hpp"
bool Orbwalker::AttackReady(int sleep)
{
	if (Engine::GetGameTime() + (Engine::GetPing() / 2 + sleep) * 0.001f >= attackTimer + Me->GetAttackDelay())
		return true;


	return false;
}

bool Orbwalker::MoveReady(int sleep)
{
	if (Engine::GetGameTime() + (Engine::GetPing() / 2) * 0.001f >= attackTimer + Me->GetAttackCastDelay() + (sleep) * 0.001f)
		return true;

	return false;
}

GameObject* Orbwalker::GetTarget(OrbwalType type)
{
	vector<GameObject*> ObjArray;
	switch (type)
	{
	case OrbwalType::Combo:
		ObjArray.assign(Game::GetIns()->HeroCache.begin(), Game::GetIns()->HeroCache.end());
		break;
	default:
		break;
	}
	GameObject* selected = nullptr;
	float max = 0;
	for (auto Obj: ObjArray)
	{
		if(Obj->IsAlive() && Me->GetTeam() != Obj->GetTeam() && Obj ->IsTargetable())
		{
			if(InAttackRange(Obj))
			{ 
				if ( (Me->CalcDamage(Obj) / (Obj->GetHealth() * 0.8f) > max))
				{
					max = (Me->CalcDamage(Obj) / Obj->GetHealth());
					selected = Obj;
				}
			}
		}
	}


	return selected;
}

bool Orbwalker::InAttackRange(GameObject* target)
{
	float myRange = Me->GetAttackRange() + Me->GetBoundingRadius();
	auto LocalPos = Me->GetPos();
	auto TargetPos = target->GetPos();
	float dist = (LocalPos - TargetPos).length();//(LocalPos.X - TargetPos.X) * (LocalPos.X - TargetPos.X) + (LocalPos.Z - TargetPos.Z) * (LocalPos.Z - TargetPos.Z);
	return dist < myRange;
}

void Orbwalker::ComBo()
{
	GameObject* Target = GetTarget(OrbwalType::Combo);

	if (Target)
	{
		if (AttackReady())
		{
			Me->Attack(Target, false);
			attackTimer = Engine::GetGameTime() + (Engine::GetPing() / 2) * 0.001f;
		}
		else if(MoveReady())
		{
			Me->MoveTo(Engine::GetMouseWorldPosition());
		}
	}
	else
	{
		attackTimer = 0.f;

	}
	
}
