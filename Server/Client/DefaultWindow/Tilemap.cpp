#include "pch.h"
#include "Tilemap.h"
#include <fstream>
#include "Object.h"
#include "BreakableBlock.h"
#include "SolidBlock.h"

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

Tile* Tilemap::GetTileAt(Vec2Int tilePos)
{
	if (tilePos.x < 0 || tilePos.x >= _mapSize.x || tilePos.y < 0 || tilePos.y >= _mapSize.y)
		return nullptr;

	return &_tiles[tilePos.y][tilePos.x];
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

void Tilemap::BuildMap()
{
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
					object = Object::CreateObject<BreakableBlock>();
					break;
				case 2:
					object = Object::CreateObject<SolidBlock>();
					break;
				}
				object->SetPos(Utils::TileToWorld({x, y}));
			}
			
		}
	}
}