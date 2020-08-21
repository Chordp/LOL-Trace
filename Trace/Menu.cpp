#include "Menu.h"

namespace Json
{

    void from_json(const nlohmann::json& j, Json::Champion& p) {
        try
        {
            p.name = j["name"];
            p.title = j["title"];
        }
        catch (const std::exception&)
        {

        }
    }
    void from_json(const nlohmann::json& j, Json::Contrast& p) {
        try
        {
            //j[u8"HeroOption"].get_to(p.HeroOption);
            j["champion"].get_to(p.Champion);
            j["Summoner"].get_to(p.Summoner);
        }
        catch (const std::exception&)
        {

        }
    }
}



void Menu::SuperShow(json& j, json& c)
{

    try
    {
        int RadioButton = 0;
        for (auto& menu : j)
        {
            bool issvae = false;
            int type = menu[0];

            switch (MenuType(type))
            {
            case MenuType::Menu:
            {
                MenuTuple x = menu;
                if (ImGui::BeginMenu(std::get<1>(x).c_str()))
                {

                    SuperShow(menu.back(), c[std::get<1>(x)]);
                    ImGui::EndMenu();
                }



            }break;
            case MenuType::Checkbox:
            {
                MenuTuple x = menu;
                auto& a = c[std::get<1>(x)];
                if (a.empty())
                    a = false;
                auto v = a.get_ptr<bool*>();
                issvae = ImGui::Checkbox(std::get<1>(x).c_str(), v);
            }break;
            case MenuType::SliderInt:
            {
                tuple<MenuType, string, int, int, DWORD> x = menu;
                auto& a = c[std::get<1>(x)];
                if (a.empty())
                    a = 0;
                auto v = (int*)a.get_ptr<__int64*>();




                if (ImGui::SliderInt(std::get<1>(x).c_str(), v, std::get<2>(x), std::get<3>(x)))
                {
                    auto CallBack = reinterpret_cast<void*>(std::get<4>(x));
                    if (CallBack != nullptr)
                        reinterpret_cast<SliderIntCallBack>(CallBack)(v);
                    issvae = true;
                }
            }break;
            case MenuType::SameLine:
            {
                ImGui::SameLine();
            }break;
            case MenuType::Map:
            {
                MenuTuple x = menu;
                SuperShow(menu.back(), c[std::get<1>(x)]);
            }break;
            case MenuType::BulletText:
            {
                MenuTuple x = menu;
                ImGui::BulletText(std::get<1>(x).c_str());
            }break;
            case MenuType::ColorEdit4:
            {
                MenuTuple x = menu;
                auto& a = c[std::get<1>(x)];
                if (a.empty())
                    a = IM_COL32(255, 255, 255, 255);
                ImU32 v = a;
                ImVec4 color;
                color = ImColor(v);
                issvae = ImGui::ColorEdit4(std::get<1>(x).c_str(), (float*)&color, ImGuiColorEditFlags_AlphaBar);
                a = v = ImColor(color);
            }break;
            case MenuType::RadioButton:
            {
                tuple<MenuType, string, string> x = menu;
                auto& a = c[std::get<1>(x)];
                if (a.empty())
                    a = 0;
                auto v = (int*)a.get_ptr<INT64*>();
                issvae =  ImGui::RadioButton(std::get<2>(x).c_str(), v, RadioButton++);
            }
            default:
                break;
            }
            if (issvae) Save();
        }
    }
    catch (const std::exception&)
    {

    }
}
tuple<MenuType, string, string> Menu::RadioButton(string name, string v)
{
    return  std::make_tuple(MenuType::RadioButton, v, name);
}
void Menu::Show()
{
    if (!Open)return;
    ImGui::Begin(u8"…µ±∆÷˙ ÷", nullptr, ImGuiWindowFlags_NoCollapse);
    SuperShow(control,config);
    ImGui::End();
}

json& Menu::AddMenu(string name)
{

    return AddMenu(control, name);
}

json Menu::SameLine()
{
    

    return{ MenuType::SameLine };
}

string Menu::GetPath()
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\FuckTp", NULL, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        BYTE buffer[500] = "";
        DWORD dwSize = 500;
        if (RegQueryValueEx(hKey, "Path", NULL, NULL, buffer, &dwSize) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return (char*)buffer;
        }

        RegCloseKey(hKey);
    }

    return string();

    
}




void Menu::Save()
{

    ofstream outFile(GetPath() + "Trace.json");
    if (outFile.is_open())
    {

        outFile << config;
        outFile.close();
    }

}

void Menu::Load()
{

    try
    {
        string Path = GetPath();
        nlohmann::json j;
        ifstream InFile(Path + "Trace.json");
        if (InFile.is_open())
            InFile >> config;
        InFile.close();
        InFile.open(Path + "contrast.json");
        if (InFile.is_open())
            InFile >> j;
        Contrast = j;
        InFile.close();
    }
    catch (const std::exception&)
    {

    }
}

tuple<MenuType,string> Menu::Checkbox(string name)
{
    return std::make_tuple(MenuType::Checkbox, name);
}



tuple<MenuType, string, int, int, DWORD> Menu::SliderInt(string name, int min, int max, SliderIntCallBack callback)
{
    return std::make_tuple(MenuType::SliderInt, name, min, max, (DWORD)(callback));
}



MenuTuple Menu::ColorEdit4(string name)
{
    return MenuTuple(MenuType::ColorEdit4,name);
}

json& Menu::AddMenu(json& j, string name)
{
    j.push_back(std::make_tuple((int)MenuType::Menu, name, json{}));
    return j.back().back();
}

tuple<MenuType, string, json> Menu::AddMenu(string name, json j)
{
  
        return std::make_tuple(MenuType::Menu, name, j);

}

tuple<MenuType, string, json> Menu::Map(string name, json j)
{
    return std::make_tuple(MenuType::Map, name, j);
}

MenuTuple Menu::BulletText(const char* message, ...)
{
    char text[1024] = {};
    va_list args;
    va_start(args, message);
    vsprintf_s(text, message, args);
    va_end(args);
    return MenuTuple(MenuType::BulletText, text);
}
