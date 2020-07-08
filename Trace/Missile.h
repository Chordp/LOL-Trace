#pragma once
#include "GameObject.hpp"
class GameObject;
class Missile :public GameObject
{	public:
	Vector GetSpellStartPos();
	Vector GetSpellEndPos();
	float GetSpellWidth();
	bool IsBasicAttack();
	bool IsAutoAttack();
	int GetSourceIndex();
};

