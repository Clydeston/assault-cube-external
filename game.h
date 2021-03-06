#pragma once
#include <Windows.h>
#include "offsets.h"
#include "mem.h"
#include "proc.h"
#include "vector.h"

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
				Vec2 vScreen;
				Vec2 vHead;
		};
		class Entity
		{
			public:
				uintptr_t baseAddress = 0;
				BYTE team;
				char name[16];
				int health;
				void GetEntityInfo(Game game, int entityListPosition);
				Vec3 GetEntityHeadPos(Entity entity, Game game);
				Vec3 GetEntityPos(Entity entity, Game game);
		};
		Entity playerList[32];
		int playerCount;
		HANDLE hProcess = 0;
		uintptr_t moduleBaseAddress = 0;
		bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight);
		float Matrix[16];
		HWND hWnd;
		HDC HDC;
};



