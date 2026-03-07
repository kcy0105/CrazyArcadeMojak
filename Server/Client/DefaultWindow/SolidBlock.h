#pragma once
#include "BlockingObject.h"
class SolidBlock : public BlockingObject
{
public:
	SolidBlock() {}
	virtual ~SolidBlock() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;
};

