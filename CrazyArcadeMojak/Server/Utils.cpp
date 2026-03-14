#include "pch.h"
#include "Utils.h"

Vec2Int Utils::WorldToTile(Vec2 worldPos)
{
	return Vec2Int((int32)(worldPos.x / TILE_SIZE), (int32)(worldPos.y / TILE_SIZE));
}

Vec2 Utils::TileToWorld(Vec2Int tilePos)
{
	return Vec2(tilePos.x * TILE_SIZE + TILE_SIZE / 2, tilePos.y * TILE_SIZE + TILE_SIZE / 2);
}
