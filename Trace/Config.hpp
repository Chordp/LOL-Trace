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
	/// <summary>
	/// 单例模式,获取实例
	/// </summary>
	/// <returns></returns>
	static auto GetIns()
	{
		static auto _Config = new Config();
		return _Config;
	}
	/// <summary>
	/// 加载配置项
	/// </summary>
	/// <returns>Json::Setting</returns>
	Json::Setting Loader();
	/// <summary>
	/// 保存配置项,保存指定配置
	/// </summary>
	/// <param name="p"></param>
	/// <returns></returns>
	static bool Save(Json::Setting p);
	/// <summary>
	/// 保存配置项,保存默认配置
	/// </summary>
	/// <returns></returns>
	bool Save();
};

