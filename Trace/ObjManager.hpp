#pragma once
#include "Trace.h"
#include "Engine.hpp"
#include "GameObject.hpp"
class GameObject;
class Hero;
class ObjManager
{
public:
	GameObject* GetFirstObject();
	GameObject* GetNextObject(GameObject* object);
	vector<GameObject*> GetAllObject();
	vector<Hero*> GetHeroList();
};

