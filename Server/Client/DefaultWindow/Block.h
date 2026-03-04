#pragma once
#include "Object.h"
class Block : public Object
{
public:
	Block() {}
	virtual ~Block() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;
};

