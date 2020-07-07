#include "ObjManager.hpp"

GameObject* ObjManager::GetFirstObject()
{
	static auto fnGetFirst = reinterpret_cast<GameObject * (__thiscall*)(ObjManager*)>(Engine::GetBaseModule()+Function::GetFirstObj);
	return fnGetFirst(this);
}

GameObject* ObjManager::GetNextObject(GameObject* object)
{
	static auto fnGetNext = reinterpret_cast<GameObject * (__thiscall*)(ObjManager*,GameObject*)>(Engine::GetBaseModule() + Function::GetNextObj);
	return fnGetNext(this, object);
	return nullptr;
}

vector<GameObject*> ObjManager::GetAllObject()
{
	vector<GameObject*> Object;
	auto obj = GetFirstObject();
	while (obj)
	{
		Object.push_back(obj);
		obj = GetNextObject(obj);
	}
	return  Object;
}

vector<Hero*> ObjManager::GetHeroList()
{
	vector<Hero*> Object;
	auto obj = GetFirstObject();
	while (obj)
	{
		if(obj->IsHero())
		{ 
			Object.push_back((Hero*)obj);
		}
		obj = GetNextObject(obj);
	}
	return Object;
}


//vector<Missile*> ObjManager::GetMissileList()
//{
//	vector<Missile*> Object;
//	auto obj = GetFirstObject();
//	while (obj)
//	{
//		if (obj->IsMissile())
//		{
//			Object.push_back((Missile*)obj);
//		}
//		obj = GetNextObject(obj);
//	}
//	return Object;
//}
