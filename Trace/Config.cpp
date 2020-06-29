#include "Config.hpp"
#include <fstream>

namespace Json 
{

	void to_json(nlohmann::json& j, const Hero& p) { j["Skin"] = p.Skin; }
	void from_json(const nlohmann::json& j, Hero& p) { p.Skin = j[u8"Skin"]; }

	void to_json(nlohmann::json& j, const Setting& p)
	{
		j["Path"] = p.Path;
		j["Gank"] = p.Gank;
		j["DrawCd"] = p.DrawCd;
		j["HeroOption"] = p.HeroOption;
	}
	void from_json(const nlohmann::json& j, Setting& p) {
		try
		{
			p.Path = j["Path"];
			p.Gank = j["Gank"];
			p.DrawCd = j["DrawCd"];
			j["HeroOption"].get_to(p.HeroOption);
				
		}
		catch (const std::exception&)
		{

		}
		

	}

	void from_json(const nlohmann::json& j, Contrast& p) {
		try
		{
			//j[u8"HeroOption"].get_to(p.HeroOption);
			p.name = j["name"];
			p.title = j["title"];
		}
		catch (const std::exception&)
		{

		}


	}

}



Json::Setting Config::Loader()
{
	try
	{
		nlohmann::json j;
		ifstream InFile("Trace.json");
		if (InFile.is_open())
			InFile >> j;
		InFile.close();
		Setting = j;
		j.clear();
		InFile.open("contrast.json");
		if (InFile.is_open())
			InFile >> j;
	
		j.get_to(Contrast);
		InFile.close();
	}
	catch (const std::exception&)
	{

	}
	
	return Setting;
}
bool Config::Save(Json::Setting p)
{
	nlohmann::json j = p;
	ofstream outFile("Trace.json");
	if (outFile.is_open())
	{
		//cout << j << endl;
		outFile << j;
		outFile.close();
		return true;
	}
	return false;

	
}

bool Config::Save()
{

	return Save(Setting);
}