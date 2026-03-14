#pragma once
#include "BlockingObject.h"
class BreakableBlock : public BlockingObject
{
public:
	BreakableBlock() {}
	virtual ~BreakableBlock() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;
};

