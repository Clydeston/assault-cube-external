#include "game.h"
#include "vector.h"

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

Vec3 Game::Entity::GetEntityPos(Game::Entity entity, Game game)
{
	uintptr_t entityXAddress = entity.baseAddress + entity_x;
	uintptr_t entityYAddress = entity.baseAddress + entity_y;
	uintptr_t entityZAddress = entity.baseAddress + entity_z;

	float x = mem::RPM<float>(entityXAddress, game.hProcess);
	float y = mem::RPM<float>(entityYAddress, game.hProcess);
	float z = mem::RPM<float>(entityZAddress, game.hProcess);
	Vec3 pos = { x, y, z};

	return pos;
}

Vec3 Game::Entity::GetEntityHeadPos(Game::Entity entity, Game game)
{
	uintptr_t entityXAddress = entity.baseAddress + entity_head_x;
	uintptr_t entityYAddress = entity.baseAddress + entity_head_y;
	uintptr_t entityZAddress = entity.baseAddress + entity_head_z;

	float x = mem::RPM<float>(entityXAddress, game.hProcess);
	float y = mem::RPM<float>(entityYAddress, game.hProcess);
	float z = mem::RPM<float>(entityZAddress, game.hProcess);
	Vec3 pos = { x, y, z };

	return pos;
}

bool Game::WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight) // 3D to 2D
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vec4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	//Transform to window coordinates
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);	
	return true;
}