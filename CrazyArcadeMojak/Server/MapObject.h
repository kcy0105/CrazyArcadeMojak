#pragma once
#include "Object.h"
class MapObject : public Object
{
public:
	MapObject(MAP_OBJECT_TYPE type) : Object(OBJECT_TYPE_MAP_OBJECT), _mapObjectType(type) {}
	virtual ~MapObject() {}

public:
	MAP_OBJECT_TYPE GetMapObjectType()	const { return _mapObjectType; }
	virtual RECT GetRect()				
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

