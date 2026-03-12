#pragma once
#include "Object.h"
class MapObject : public Object
{
public:
	void SetMapObjectType(MAP_OBJECT_TYPE type) { _mapObjectType = type; }
	MAP_OBJECT_TYPE GetMapObjectType()	const { return _mapObjectType; }
	RECT GetRect()						const
	{
		return {	(LONG)_pos.x - TILE_SIZE / 2,
					(LONG)_pos.y - TILE_SIZE / 2,
					(LONG)_pos.x + TILE_SIZE / 2 ,
					(LONG)_pos.y + TILE_SIZE / 2 };
	}

private:
	MAP_OBJECT_TYPE _mapObjectType;

public:
	virtual bool BlocksPlayer(const Player* player) const { return true; }
};

