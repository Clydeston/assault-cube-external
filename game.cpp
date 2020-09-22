#include "game.h"

void Game::LocalPlayer::GetLocalPlayerInfo(Game game)
{
	baseAddress = game.moduleBaseAddress + local_entity_object;

	healthAddress = FindDMAAddy(game.hProcess, baseAddress, { entity_health });
	armourAddress = FindDMAAddy(game.hProcess, baseAddress, { entity_armour });
	uintptr_t playerTeamAddress = FindDMAAddy(game.hProcess, baseAddress, { entity_team });
	uintptr_t playerNameAddress = FindDMAAddy(game.hProcess, baseAddress, { entity_name });

	std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
	uintptr_t ammoAddress = FindDMAAddy(game.hProcess, baseAddress, ammoOffsets);

	team = mem::RPM<BYTE>(playerTeamAddress, game.hProcess);
	name[16] = mem::RPM<char>(playerNameAddress, game.hProcess);
	health = mem::RPM<int>(healthAddress, game.hProcess);
	ammo = mem::RPM<int>(ammoAddress, game.hProcess);
}

void Game::Entity::GetEntityInfo(Game game, int entityListPosition)
{
	unsigned int entitySizeOffset = (entityListPosition + 1) * entity_byte_size;
	uintptr_t entityListAdr = game.moduleBaseAddress + entity_list;
	baseAddress = FindDMAAddy(game.hProcess, entityListAdr, { entitySizeOffset });

	uintptr_t entityNameAdr = FindDMAAddy(game.hProcess, entityListAdr, { entitySizeOffset, entity_name });
	uintptr_t entityHealthAdr = FindDMAAddy(game.hProcess, entityListAdr, { entitySizeOffset, entity_health });
	uintptr_t entityTeamAdr = FindDMAAddy(game.hProcess, entityListAdr, { entitySizeOffset, entity_team });

	ReadProcessMemory(game.hProcess, (BYTE*)entityNameAdr, &name, sizeof(16), nullptr);

	team = mem::RPM<BYTE>(entityTeamAdr, game.hProcess);
	name[16] = mem::RPM<char>(entityNameAdr, game.hProcess);
	health = mem::RPM<int>(entityHealthAdr, game.hProcess);
}