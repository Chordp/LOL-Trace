#pragma once
#include "Trace.h"
#include <nlohmann/json.hpp>
#include <tuple>;
#include <fstream>
using namespace std;
using namespace nlohmann;


namespace Json
{

	struct Champion
	{
		string name;
		string title;

	};
	struct Contrast
	{
		map<string, Json::Champion> Champion;
		map<string, vector<BYTE>> Summoner;

	};

}
enum class MenuType :int
{
    Menu,
    Checkbox,
    SliderInt,
    SameLine,
    Map,
    BulletText,
    ColorEdit4,
    RadioButton
};
using MenuTuple = tuple<MenuType, string>;
using SliderIntCallBack = void(*)(int*);
class Menu
{
   json control;
   void SuperShow(json& j,json &c);
   
public :
    bool Open = false;
    Json::Contrast Contrast;
    json config;
public:
    static auto GetIns()
    {
        static auto _menu = new Menu();
        return _menu;
    }
    static MenuTuple  Checkbox(string name);
    static tuple<MenuType, string, string> RadioButton(string name, string v);
    static tuple<MenuType, string, int, int, DWORD> SliderInt(string name, int min, int max, SliderIntCallBack callback = nullptr);
 
    static MenuTuple ColorEdit4(string name);
    static json& AddMenu(json& j, string name);
    static tuple<MenuType, string, json> AddMenu(string name, json j);
    static tuple<MenuType, string, json> Map(string name, json j);
    static MenuTuple BulletText(const char* message, ...);
    json SameLine();
public:
    void Show();
    json& AddMenu(string name);
    

    void Save();
    void Load();
};

