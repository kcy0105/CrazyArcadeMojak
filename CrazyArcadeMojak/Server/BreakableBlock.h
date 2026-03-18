#pragma once
#include "MapObject.h"
class BreakableBlock : public MapObject
{
public:
    BreakableBlock() : MapObject(MAP_OBJECT_TYPE_BREAKABLE_BLOCK) {}
    virtual ~BreakableBlock() {}
};

