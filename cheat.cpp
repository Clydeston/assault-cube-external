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
