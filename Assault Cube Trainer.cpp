#include <iostream>
#include <Windows.h>
#include "proc.h"
#include "mem.h"
#include "offsets.h"
#include "game.h"
#include "cheat.h"

std::string BoolToText(bool bBool)
{
	if (bBool)
	{
		return "Enabled";
	}
	return "Disabled";
}

void updateConsole(Cheat::Settings settings)
{
	system("cls");
	std::cout << (settings.bHealth ? "\033[32m" : "\033[31m") << "Godmode NUM1: " << BoolToText(settings.bHealth) << "\033[0m" << std::endl;
	std::cout << (settings.bAmmo ? "\033[32m" : "\033[31m") << "Max Ammo NUM2: " << BoolToText(settings.bAmmo) << "\033[0m" << std::endl;
	std::cout << (settings.bRecoil ? "\033[32m" : "\033[31m") << "No Recoil NUM3: " << BoolToText(settings.bRecoil) << "\033[0m" << std::endl;
	std::cout << (settings.bArmour ? "\033[32m" : "\033[31m") << "Max Armour NUM4: " << BoolToText(settings.bArmour) << "\033[0m" << std::endl;
	
}

int main()
{
	// getting proc id of process
	DWORD procID = GetProcessId(L"ac_client.exe");
	Game game;
	Cheat cheat;
	Cheat::Settings settings;	
	
	game.hProcess = 0;
	game.moduleBaseAddress = 0;

	if (procID)
	{
		// getting handle to process
		game.hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

		if (game.hProcess != INVALID_HANDLE_VALUE && game.hProcess != 0x0)
		{
			// getting module base address
			game.moduleBaseAddress = GetModuleBaseAddress(procID, L"ac_client.exe");			

			Game::LocalPlayer localPlayer;
			localPlayer.baseAddress = game.moduleBaseAddress + local_entity_object;
			localPlayer.GetLocalPlayerInfo(game);

			cheat.gameObject = game;
			cheat.localPlayer = localPlayer;

			updateConsole(settings);
			std::cout << "Base Pointer" << std::hex << localPlayer.baseAddress << std::endl;

			/*
			 HACK LOOP
			*/
			DWORD dwExit = 0;
			while (GetExitCodeProcess(game.hProcess, &dwExit) && dwExit == STILL_ACTIVE)
			{
				// HEALTH
				if (GetAsyncKeyState(VK_NUMPAD1) & 1)
				{
					settings.bHealth = !settings.bHealth;
					updateConsole(settings);
				}

				// AMMO
				if (GetAsyncKeyState(VK_NUMPAD2) & 1)
				{
					settings.bAmmo = !settings.bAmmo;
					updateConsole(settings);
					cheat.MaxAmmo(settings.bAmmo, game);
				}

				// RECOIL
				if (GetAsyncKeyState(VK_NUMPAD3) & 1)
				{
					settings.bRecoil = !settings.bRecoil;
					updateConsole(settings);
					cheat.NoRecoil(settings.bRecoil, game);
				}

				// ARMOUR
				if (GetAsyncKeyState(VK_NUMPAD4) & 1)
				{
					settings.bArmour = !settings.bArmour;
					updateConsole(settings);
				}

				// Health
				if (settings.bHealth)
				{
					cheat.GodMode(settings.newValue);					
					}

				// ARMOUR
				if (settings.bArmour)
				{
					cheat.MaxArmour(settings.newValue);
				}

				Sleep(10);
			}
		}
		else
		{
			std::cout << "Invalid Process Handle";			
			return 0;
			getchar();
		}
	}
	else
	{
		std::cout << "Couldn't Find Proc ID";		
		return 0;
		getchar();
	}

	std::cout << "Couldn't Find Process";	
	return 0;
	getchar();
}

/*

// getting updated player count
						//game.playerCount = mem::RPM<int>((game.moduleBaseAddress + entity_count), game.hProcess);
						// TODO updated count to playercount
						for (unsigned int i = 0; i < 32; i++)
						{
							game.playerList[i].GetEntityInfo(game, i);
							// health of invalid entities are stupidly negative or stupidly large
							if (game.playerList[i].health < 0 || game.playerList[i].health > 1337)
							{
								continue;
							}
							game.playerCount += 1;
							std::cout << "Health"<< std::dec << game.playerList[i].health << std::endl;
							//std::cout << std::dec << game.playerList[i].name << std::endl;
						}


*/
