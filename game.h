#pragma once
#include <Windows.h>
#include "offsets.h"
#include "mem.h"
#include "proc.h"

class Game
{
	public : 		
		class LocalPlayer
		{
			public :
				uintptr_t baseAddress = 0;
				uintptr_t healthAddress = 0;
				uintptr_t armourAddress = 0;
				BYTE team;
				char name[16];
				int health;
				int ammo;
				void GetLocalPlayerInfo(Game game);
		};
		class Entity
		{
			public:
				uintptr_t baseAddress = 0;
				BYTE team;
				char name[16];
				int health;
				void GetEntityInfo(Game game, int entityListPosition);
		};
		Entity playerList[32];
		int playerCount;
		HANDLE hProcess = 0;
		uintptr_t moduleBaseAddress = 0;
};



