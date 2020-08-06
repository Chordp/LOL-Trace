#pragma once
#include "Engine.hpp"
class AIO
{
	AIO();
	
public:
	static auto GetIns()
	{
		static auto _aio = new AIO();
		return _aio;
	}


	void GankTips(tuple<bool, float, int, int>& v);

	void DrawCD();

	void DrawPath(tuple<bool, float, int>& v);


	static void Present();
};

