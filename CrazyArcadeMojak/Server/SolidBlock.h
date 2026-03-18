#pragma once
#include "MapObject.h"
class SolidBlock : public MapObject
{
public:
	SolidBlock() : MapObject(MAP_OBJECT_TYPE_SOLID_BLOCK) {}
	virtual ~SolidBlock() {}
};

