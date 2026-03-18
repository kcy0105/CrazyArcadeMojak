#pragma once
#include "MapObject.h"
class BreakableBlock : public MapObject
{
public:
	BreakableBlock() {}
	virtual ~BreakableBlock() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
};

