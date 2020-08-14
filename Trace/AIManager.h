#pragma once
#include "Vector.h"
#include "Offset.h"
#include <vector>
class AIManager
{
public:
	Vector GetVelocity();

	bool HasNavPath();

	bool IsMoving();
	Vector** GetNavigationStart();

	Vector** GetNavigationEnd();

	std::vector<Vector>GetPathList();
	Vector GetDestination();
};

