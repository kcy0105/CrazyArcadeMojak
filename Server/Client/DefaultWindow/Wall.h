#pragma once
#include "Object.h"
class Wall : public Object
{
public:
	Wall() {}
	virtual ~Wall() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;
};

