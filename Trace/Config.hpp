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
		bool DrawCd;
		bool Gank;
		bool Path;
		map<string, Hero> HeroOption;
	};
	struct Contrast
	{
		string name;
		string title;

	};


}
class Config
{
private:
	Config() {};
public:
	map<string,Json::Contrast> Contrast;
	Json::Setting Setting;
public:
	/// <summary>
	/// ����ģʽ,��ȡʵ��
	/// </summary>
	/// <returns></returns>
	static auto GetIns()
	{
		static auto _Config = new Config();
		return _Config;
	}
	/// <summary>
	/// ����������
	/// </summary>
	/// <returns>Json::Setting</returns>
	Json::Setting Loader();
	/// <summary>
	/// ����������,����ָ������
	/// </summary>
	/// <param name="p"></param>
	/// <returns></returns>
	static bool Save(Json::Setting p);
	/// <summary>
	/// ����������,����Ĭ������
	/// </summary>
	/// <returns></returns>
	bool Save();
};

