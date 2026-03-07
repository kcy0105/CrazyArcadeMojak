#include "pch.h"
#include "MapObject.h"
#include "BreakableBlock.h"
#include "SolidBlock.h"
#include "WaterBomb.h"

void MapObject::OnInit()
{
}

void MapObject::OnUpdate()
{
}

void MapObject::OnRender(HDC hdc)
{
}

void MapObject::OnRelease()
{
}

MapObject* MapObject::Factory(MAP_OBJECT_TYPE type)
{
	switch (type)
	{
	case MAP_OBJECT_TYPE_BREAKABLE_BLOCK:
		return Object::CreateObject<BreakableBlock>();
	case MAP_OBJECT_TYPE_SOLID_BLOCK:
		return Object::CreateObject<SolidBlock>();
	case MAP_OBJECT_TYPE_WATER_BOMB:
		return Object::CreateObject<WaterBomb>();
	}

	return nullptr;
}