#include "AIManager.h"

Vector AIManager::GetVelocity() {
	return *(Vector*)((DWORD)this + AIMGR::VELOCITY);
}

bool AIManager::HasNavPath() {
	return *(BYTE*)((DWORD)this + AIMGR::HASNAVPATH) == 1;
}

bool AIManager::IsMoving() {
	return *(bool*)((DWORD)this + AIMGR::ISMOVING);
}

Vector** AIManager::GetNavigationStart() {
	return (Vector**)((DWORD)this + AIMGR::NAVBEGIN);
}

Vector** AIManager::GetNavigationEnd() {
	return (Vector**)((DWORD)this + AIMGR::NAVEND);
}

std::vector<Vector> AIManager::GetPathList()
{
	std::vector<Vector> bufflist;

	const auto buffBegin = *GetNavigationStart();
	const auto buffEnd = *GetNavigationEnd();
	if (buffBegin != nullptr && buffEnd != nullptr)
	{
		for (uint32_t i = 0; i < (buffEnd - buffBegin); i++)
		{
			const auto buffNode = buffBegin + (i);

			if (buffNode != nullptr && !IsBadReadPtr(buffNode, 12))
			{
				if (buffNode->X > 20 && buffNode->Y > 20 && buffNode->Z > 20 && buffNode->X < 50000)
				{
					//Console.print("%f , %f , %f \n", buffNode->X, buffNode->Y, buffNode->Z);

					bufflist.push_back(*buffNode);
				}

			}
		}



	}

	return bufflist;
}
