#pragma once
#include "MapObject.h"
class BlockingObject : public MapObject
{
public:
	BlockingObject(MAP_OBJECT_TYPE type) : MapObject(type) {}
	virtual ~BlockingObject() {}
};

