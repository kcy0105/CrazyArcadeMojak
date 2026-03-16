#pragma once
#include "MapObject.h"
class BlockingObject : public MapObject
{
public:
	BlockingObject() {}
	virtual ~BlockingObject() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
};

