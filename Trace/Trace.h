#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <d3d9.h>
#include "Vector.h"
using namespace std;
#include "Offset.h"


#include "utlis.hpp"
#include "EventHandle.h"
#include "ImGuiRendering.h"
#include "Menu.h"

#include "Color.hpp"
#include "Engine.hpp"
#include "GameObject.hpp"



#define Draw ImGuiRendering::GetIns()
#define Me Engine::GetLocalPlayer()
//#define setting Config::GetIns()->Setting