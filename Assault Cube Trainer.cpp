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
	std::cout << (settings.bEsp ? "\033[32m" : "\033[31m") << "ESP NUM5: " << BoolToText(settings.bEsp) << "\033[0m" << std::endl;
	
}

int main()
{
	// getting proc id of process
	DWORD procID = GetProcessId(L"ac_client.exe");
	Game game;
	Cheat cheat;
	Cheat::Drawing drawing;
	Cheat::Settings settings;	
	
	game.hProcess = 0;
	game.moduleBaseAddress = 0;

	if (procID)
	{
		// getting handle to process
		game.hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);
		game.hWnd = FindWindow(0, L"AssaultCube");

		if (game.hProcess != INVALID_HANDLE_VALUE && game.hProcess != 0x0)
		{
			// getting module base address
			game.moduleBaseAddress = GetModuleBaseAddress(procID, L"ac_client.exe");	
			//game.HDC = GetDC(game.hWnd);

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
				ReadProcessMemory(game.hProcess, (BYTE*)view_matrix, &game.Matrix, sizeof(game.Matrix), nullptr);				
				game.HDC = GetDC(game.hWnd);

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

				// ESP
				if (GetAsyncKeyState(VK_NUMPAD5) & 1)
				{
					settings.bEsp = !settings.bEsp;		
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

				// ESP
				if (settings.bEsp)
				{
					// loop entities 
					for (unsigned int i = 0; i < 32; i++)
					{
						game.playerList[i].GetEntityInfo(game, i);

						// health of invalid entities are stupidly negative or stupidly large
						if (game.playerList[i].health < 0 || game.playerList[i].health > 1337)
						{
							continue;
						}
						game.playerCount += 1;

						if (game.playerList[i].team != localPlayer.team)
						{
							std::cout << "Name" << game.playerList[i].name << std::endl;
							// get enemy position
							Vec3 enemyPos = game.playerList[i].GetEntityPos(game.playerList[i], game);
							// get enemy head position
							Vec3 enemyHeadPos = game.playerList[i].GetEntityHeadPos(game.playerList[i], game);

							//wlrd to scrn enemy pos
							if (game.WorldToScreen(enemyPos, localPlayer.vScreen, game.Matrix, 1920, 1080))
							{
								//wlrd to scrn enemy head pos
								if (game.WorldToScreen(enemyHeadPos, localPlayer.vHead, game.Matrix, 1920, 1080))
								{
									//Creates the head height
									float head = localPlayer.vHead.y - localPlayer.vScreen.y;
									//Creates Width
									float width = head / 2;
									//Creates Center
									float center = width / -2;
									//Creates Extra area above head
									float extra = head / -6;

									//draw	
									drawing.Brush = CreateSolidBrush(RGB(158, 66, 244));
									drawing.DrawBorderBox(localPlayer.vScreen.x + center, localPlayer.vScreen.y, width, head - extra, 1, game);									
									DeleteObject(drawing.Brush);
								}
							}
						}
																		
						//std::cout << "Health" << std::dec << game.playerList[i].health << std::endl;
						//std::cout << std::dec << game.playerList[i].name << std::endl;
					}
				}
				Sleep(1);
				DeleteObject(game.HDC);
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
