#include "Skin.h"
#include "Menu.h"
#include "Hero.hpp"
#define Config Menu::GetIns()->config


Skin::Skin()
{
	Menu::GetIns()->AddMenu(u8"Æ¤·ô") =
	{
		Menu::Map(
			Me->GetChampionName(),
			{
				Menu::SliderInt(u8"Skin",0,100,[](int* v)->void
				{
					Me->SetCharacter(*v);
				}),
				Menu::BulletText(u8"PgUp | PgDn »»·ô")
			}

		)
	};

	auto& SkinID = Config[u8"Æ¤·ô"][Me->GetChampionName()][u8"Skin"];
	if (SkinID.empty())
	{
		SkinID = 0;
	}
	else
	{
		Me->SetCharacter(SkinID);
	}

	
}

void Skin::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{

		switch (wParam)
		{
		case VK_PRIOR: {
			auto& skin = Config[u8"Æ¤·ô"][Me->GetChampionName()][u8"Skin"];
			auto& SkinID = skin.get_ref<INT64&>();
			Me->SetCharacter(++SkinID);

			break;
		}
		case VK_NEXT:
		{
			auto& skin = Config[u8"Æ¤·ô"][Me->GetChampionName()][u8"Skin"];
			auto& SkinID = skin.get_ref<INT64&>();
			Me->SetCharacter(--SkinID);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

