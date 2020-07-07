#pragma once
#include "Trace.h"
#define ObjManager Engine::GetObjManager()


class Game
{
private:
	vector<Hero*> HeroCache;
	Game() {};
public:
	void GetCache()
	{
		HeroCache = ObjManager->GetHeroList();
	}
	static auto GetIns()
	{
		static auto _Game = new Game();
		return _Game;
	}
	void GankTips();
	void DrawCD();
	void DrawPath();
};

