#pragma once
#include "Trace.h"

class CObject;
class ObjManager
{
public:
	CObject* GetFirstObject();
	CObject* GetNextObject(CObject* object);
	vector<CObject*> GetAllObject();
	vector<CObject*> GetAllHero();
};

