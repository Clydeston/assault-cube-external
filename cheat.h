#pragma once
#include <iostream>
#include "offsets.h"
#include "mem.h"
#include "proc.h"
#include "game.h"

class Cheat
{
	public:
		void GodMode(unsigned int newValue);
		void MaxArmour(unsigned int newValue);
		void MaxAmmo(bool bAmmo, Game game);
		void NoRecoil(bool bRecoil, Game game);		
		void ESP(bool bEsp, Game game);
		void UpdateObjects(Game gameObject, Game::LocalPlayer localPlayer);
		class Settings
		{
			public: 
				bool bHealth = false, bAmmo = false, bRecoil = false, bName = false, bSpeed = false, bArmour = false, bEsp = false;
				const unsigned int newValue = 1337;				
		 };
		class Drawing
		{
			public:
				HBRUSH Brush;
				void DrawBorderBox(int x, int y, int w, int h, int thickness, Game game);
				void DrawFilledRect(int x, int y, int w, int h, Game game);
		};
		Game gameObject;
		Game::LocalPlayer localPlayer;
};

