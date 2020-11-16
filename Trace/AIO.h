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


	static void  GankTips(tuple<bool, int, int, ImU32>& v);

	static void DrawCD();

	static void DrawPath(tuple<bool, int, ImU32>& v);


	static void Present();
};

