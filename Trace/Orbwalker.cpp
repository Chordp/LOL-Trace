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

GameObject* Orbwalker::GetTarget(vector<GameObject*> targets)
{
	//if (targets.empty())
	//	return nullptr;
	//GameObject* bestTarget = targets[0];
	////for (size_t i = 1; i < targets.size(); i++)
	////{
	////	if(targets[i]->GetHealth() < )
	////}
	return nullptr;
}

void Orbwalker::ComBo()
{
	Hero* Target = nullptr;

	if (!Target)
	{
		if (MoveReady())
		{
			Me->MoveTo(Engine::GetMouseWorldPosition());

			attackTimer = 10;
		}
	}
	
}
