#include "pch.h"
#include "Tilemap.h"
#include <iostream>
#include <fstream>

void Tilemap::LoadFile(const wstring& path)
{
	FILE* file = nullptr;

	::_wfopen_s(&file, path.c_str(), L"rb");
	assert(file);

	int mapSizeX;
	int mapSizeY;

	::fread(&mapSizeX, sizeof(mapSizeX), 1, file);
	::fread(&mapSizeY, sizeof(mapSizeY), 1, file);

	SetMapSize({ mapSizeX, mapSizeY });

	for (int32 y = 0; y < mapSizeY; y++)
	{
		for (int32 x = 0; x < mapSizeX; x++)
		{
			int32 value = -1;
			::fread(&value, sizeof(value), 1, file);
			_tiles[y][x].value = value;
		}
	}

	::fclose(file);
	return;
}

void Tilemap::SetMapSize(Vec2Int size)
{
	_mapSize = size;

	_tiles = vector<vector<Tile>>(size.y, vector<Tile>(size.x));

	for (int32 y = 0; y < size.y; y++)
	{
		for (int32 x = 0; x < size.x; x++)
		{
			_tiles[y][x] = Tile{ 0 };
		}
	}
}

void Tilemap::SetTileSize(int32 size)
{
	_tileSize = size;
}
