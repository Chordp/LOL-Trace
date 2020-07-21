#include "Orbwalker.h"
#include "Hero.hpp"
bool Orbwalker::CanAttack()
{
	//return Engine::GetGameTime() + Engine::GetPing() * 0.001f >= LastAATick + Me->GetAttackDelay();
	return Engine::GetGameTime() + (Engine::GetPing() / 2+ 25) * 0.001f >= LastAATick + Me->GetAttackDelay();
}

bool Orbwalker::CanMove(int sleep)
{

	//return Me->GetAttackCastDelay() < Engine::GetGameTime() - LastAATick + sleep * 0.001;
	return Engine::GetGameTime() + (Engine::GetPing() / 2) * 0.001f >= LastAATick + Me->GetAttackCastDelay() + sleep * 0.001f;


}

void Orbwalker::ResetAttackTimer(float t)
{
	//LastAATick = Engine::GetGameTime() + t;
	LastAATick = Engine::GetGameTime() + (Engine::GetPing() / 2) * 0.001  + t;
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
	float myRange = Me->GetAttackRange() + Me->GetBoundingRadius()+ target->GetBoundingRadius();
	auto LocalPos = Me->GetPos();
	auto TargetPos = target->GetPos();
	float dist = (LocalPos - TargetPos).length();//(LocalPos.X - TargetPos.X) * (LocalPos.X - TargetPos.X) + (LocalPos.Z - TargetPos.Z) * (LocalPos.Z - TargetPos.Z);
	return dist < myRange;
}

void Orbwalker::ComBo()
{
	GameObject* Target = GetTarget(OrbwalType::Combo);

	
	if (Target && CanAttack())
	{
		Me->Attack(Target);
		if (Me->GetSpellBook()->GetActiveSpellEntry())
		{
			if (Me->GetSpellBook()->GetActiveSpellEntry()->isAutoAttack())
			{
				ResetAttackTimer();
				cout << 1 << endl;
			}
		}
		
	}
	else if(CanMove())
	{
		
		if (Engine::GetGameTime() - LastMove > 0.3f)
		{
			Me->MoveTo(Engine::GetMouseWorldPosition());
			LastMove = Engine::GetGameTime();
		

		}
		
	}



	
	
}
