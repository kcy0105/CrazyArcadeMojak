#include "pch.h"
#include "Tilemap.h"
#include <iostream>
#include <fstream>
#include "Object.h"
#include "Block.h"
#include "Wall.h"

void Tilemap::LoadFile(const wstring& path)
{
	FILE* file = nullptr;

	::_wfopen_s(&file, path.c_str(), L"rb");
	assert(file);

	::fread(&_mapSize.x, sizeof(_mapSize.x), 1, file);
	::fread(&_mapSize.y, sizeof(_mapSize.y), 1, file);

	for (int32 y = 0; y < _mapSize.y; y++)
	{
		for (int32 x = 0; x < _mapSize.x; x++)
		{
			int32 value = -1;
			::fread(&value, sizeof(value), 1, file);
			_tiles[y][x].value = value;
		}
	}

	::fclose(file);
	return;
}

void Tilemap::SaveFile(const wstring& path)
{
	FILE* file = nullptr;
	_wfopen_s(&file, path.c_str(), L"wb");
	assert(file != nullptr);

	::fwrite(&_mapSize.x, sizeof(_mapSize.x), 1, file);
	::fwrite(&_mapSize.y, sizeof(_mapSize.y), 1, file);

	for (int32 y = 0; y < _mapSize.y; y++)
	{
		for (int32 x = 0; x < _mapSize.x; x++)
		{
			int32 value = _tiles[y][x].value;
			::fwrite(&value, sizeof(value), 1, file);
		}
	}

	::fclose(file);
	return;
}

Tile* Tilemap::GetTileAt(Vec2Int pos)
{
	if (pos.x < 0 || pos.x >= _mapSize.x || pos.y < 0 || pos.y >= _mapSize.y)
		return nullptr;

	return &_tiles[pos.y][pos.x];
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

void Tilemap::BuildMap()
{
	// TODO : ObjectManager

	for (int32 y = 0; y < _mapSize.y; y++)
	{
		for (int32 x = 0; x < _mapSize.x; x++)
		{
			if (_tiles[y][x].value > 0)
			{
				Object* object = nullptr;
				switch (_tiles[y][x].value)
				{
				case 1:
					object = Object::CreateObject<Block>();
					break;
				case 2:
					object = Object::CreateObject<Wall>();
					break;
				}
				object->SetPos(x * _tileSize + _tileSize / 2, y * _tileSize + _tileSize / 2);
			}
			
		}
	}
}
