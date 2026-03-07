#pragma once
#include "Object.h"
class MapObject : public Object
{
public:
	void SetMapObjectType(MAP_OBJECT_TYPE type) { _mapObjectType = type; }
	MAP_OBJECT_TYPE GetMapObjectType() { return _mapObjectType; }

private:
	MAP_OBJECT_TYPE _mapObjectType;

public:
	static MapObjectRef Factory(MAP_OBJECT_TYPE type);
};

