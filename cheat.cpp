#include "cheat.h"

void Cheat::GodMode(unsigned int newValue)
{
	mem::PatchEx((BYTE*)localPlayer.healthAddress, (BYTE*)&newValue, sizeof(newValue), gameObject.hProcess);
}

void Cheat::MaxArmour(unsigned int newValue)
{
	mem::PatchEx((BYTE*)localPlayer.armourAddress, (BYTE*)&newValue, sizeof(newValue), gameObject.hProcess);
}

void Cheat::MaxAmmo(bool bAmmo, Game game)
{
	if (bAmmo)
	{
		mem::PatchEx((BYTE*)(game.moduleBaseAddress + ammo_address), (BYTE*)ammo_increase_instruction, 2, game.hProcess);
	}
	else
	{
		mem::PatchEx((BYTE*)(game.moduleBaseAddress + ammo_address), (BYTE*)ammo_decrease_instruction, 2, game.hProcess);
	}
}

void Cheat::NoRecoil(bool bRecoil, Game game)
{
	if (bRecoil)
	{
		mem::NopEx((BYTE*)(game.moduleBaseAddress + recoil_address), 10, game.hProcess);
	}
	else
	{
		mem::PatchEx((BYTE*)(game.moduleBaseAddress + recoil_address), (BYTE*)original_recoil_instruction, 10, game.hProcess);
	}
}

void Cheat::UpdateObjects(Game gameObject, Game::LocalPlayer localPlayer)
{
	this->gameObject = gameObject;
	this->localPlayer = localPlayer;
}

void Cheat::ESP(bool bEsp, Game game)
{
	for (unsigned int i = 0; i < 32; i++)
	{
		game.playerList[i].GetEntityInfo(game, i);
		// health of invalid entities are stupidly negative or stupidly large
		if (game.playerList[i].health < 0 || game.playerList[i].health > 1337)
		{
			continue;
		}
		game.playerCount += 1;
		std::cout << "Health" << std::dec << game.playerList[i].health << std::endl;
		//std::cout << std::dec << game.playerList[i].name << std::endl;
	}
}

void Cheat::Drawing::DrawBorderBox(int x, int y, int w, int h, int thickness, Game game)
{
	DrawFilledRect(x, y, w, thickness, game);

	DrawFilledRect(x, y, thickness, h, game);

	DrawFilledRect((x + w), y, thickness, h, game);

	DrawFilledRect(x, y + h, w + thickness, thickness, game);
}

void Cheat::Drawing::DrawFilledRect(int x, int y, int w, int h, Game game)
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(game.HDC, &rect, Brush);
}