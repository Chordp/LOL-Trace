#pragma once
#include "Engine.hpp"

class SpellSlot
{
public:
	int GetLevel();
	float GetTime();
	bool IsReady();
	float GetCooldown();
	float GetCD();
};

