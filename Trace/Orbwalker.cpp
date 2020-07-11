#include "Orbwalker.h"

bool Orbwalker::AttackReady()
{
	if (Engine::GetGameTime() +( Engine::GetPing() / 2 + 25)* 0.001f >= attackTimer + Me->GetAttackDelay())
		return true;

		
	return false;
}

bool Orbwalker::MoveReady(float sleep)
{
	if (Engine::GetGameTime() + (Engine::GetPing() / 2) * 0.001f >= attackTimer + Me->GetAttackCastDelay()+ sleep)
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



	if (MoveReady(0.09))
	{
		Me->MoveTo(Engine::GetMouseWorldPosition());
	}
}
