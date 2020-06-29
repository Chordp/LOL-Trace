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
#include "Config.hpp"
#include "ImGuiRendering.h"
#include "Color.hpp"
#include "Engine.hpp"
#include "CObject.hpp"
#include "Hero.hpp"


#define Draw ImGuiRendering::GetIns()
#define Me Engine::GetLocalPlayer()