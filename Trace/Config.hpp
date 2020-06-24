#pragma once
#include "Trace.h"
#include <map>
#include <nlohmann/json.hpp>
namespace Json
{
	struct Hero
	{
		int Skin;
	};
	struct Setting
	{
		map<string, Hero> HeroOption;
	};


}
class Config
{
private:
	Config() {};
public:
	Json::Setting Setting;
public:
	static auto GetIns()
	{
		static auto _Config = new Config();
		return _Config;
	}
	Json::Setting Loader();
	static bool Save(Json::Setting p);
	bool Save();
};

