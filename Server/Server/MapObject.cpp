#include "pch.h"
#include "MapObject.h"
#include "BreakableBlock.h"
#include "SolidBlock.h"
#include "WaterBomb.h"

MapObjectRef MapObject::Factory(MAP_OBJECT_TYPE type)
{
	MapObjectRef obj;
	switch (type)
	{
	case MAP_OBJECT_TYPE_BREAKABLE_BLOCK:
		obj = make_shared<BreakableBlock>();
		break;
	case MAP_OBJECT_TYPE_SOLID_BLOCK:
		obj = make_shared<SolidBlock>();
		break;
	case MAP_OBJECT_TYPE_WATER_BOMB:
		obj = make_shared<WaterBomb>();
		break;
	}

	if (type > MAP_OBJECT_TYPE_NONE)
		obj->SetMapObjectType(type);

	return obj;
}
