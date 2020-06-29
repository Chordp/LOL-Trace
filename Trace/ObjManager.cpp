#include "ObjManager.hpp"

CObject* ObjManager::GetFirstObject()
{
	static auto fnGetFirst = reinterpret_cast<CObject * (__thiscall*)(ObjManager*)>(Engine::GetBaseModule()+Function::GetFirstObj);
	return fnGetFirst(this);
}

CObject* ObjManager::GetNextObject(CObject* object)
{
	static auto fnGetNext = reinterpret_cast<CObject * (__thiscall*)(ObjManager*,CObject*)>(Engine::GetBaseModule() + Function::GetNextObj);
	return fnGetNext(this, object);
	return nullptr;
}

vector<CObject*> ObjManager::GetAllObject()
{
	vector<CObject*> Object;
	auto obj = GetFirstObject();
	while (obj)
	{
		Object.push_back(obj);
		obj = GetNextObject(obj);
	}
	return  Object;
}

vector<Hero*> ObjManager::GetAllHero()
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
