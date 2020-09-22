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
		void UpdateObjects(Game gameObject, Game::LocalPlayer localPlayer);
		class Settings
		{
			public: 
				bool bHealth = false, bAmmo = false, bRecoil = false, bName = false, bSpeed = false, bArmour = false;
				const unsigned int newValue = 1337;				
		 };
		Game gameObject;
		Game::LocalPlayer localPlayer;
};

