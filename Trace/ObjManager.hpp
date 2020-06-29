#pragma once
#include "Trace.h"

class CObject;
class Hero;
class ObjManager
{
public:
	CObject* GetFirstObject();
	CObject* GetNextObject(CObject* object);
	vector<CObject*> GetAllObject();
	vector<Hero*> GetAllHero();
};

