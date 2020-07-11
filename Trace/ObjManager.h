#pragma once
#include "Trace.h"

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

