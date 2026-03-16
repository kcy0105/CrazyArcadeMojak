#pragma once
#include "BlockingObject.h"
class BreakableBlock : public BlockingObject
{
public:
    BreakableBlock() : BlockingObject(MAP_OBJECT_TYPE_BREAKABLE_BLOCK) {}
    virtual ~BreakableBlock() {}
};

