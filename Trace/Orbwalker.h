#pragma once
#include "Game.h"

class Orbwalker
{
private:
	float attackTimer = 0.f;
public:
	static auto GetIns()
	{
		static auto _Orbwalker = new Orbwalker();
		return _Orbwalker;
	}
	bool AttackReady(int sleep = 25);
	bool MoveReady(int sleep = 10);
	GameObject* GetTarget(vector<GameObject*> targets);
	void ComBo();
};

