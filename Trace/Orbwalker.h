#pragma once
#include "Game.h"
enum class OrbwalType
{
	Combo
};
class Orbwalker
{
private:
	float LastAATick = 0;
	float LastMove = 0;
public:
	static auto GetIns()
	{
		static auto _Orbwalker = new Orbwalker();
		return _Orbwalker;
	}
	bool CanAttack();
	bool CanMove(int sleep = 15);
	void ResetAttackTimer(float t = 0);
	GameObject* GetTarget(OrbwalType type);
	bool InAttackRange(GameObject* target);
	void ComBo();
};

