#pragma once
#include "BlockingObject.h"
class SolidBlock : public BlockingObject
{
public:
	SolidBlock() : BlockingObject(MAP_OBJECT_TYPE_SOLID_BLOCK) {}
	virtual ~SolidBlock() {}
};

