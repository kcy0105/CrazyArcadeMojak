#pragma once
#include "Object.h"
class MapObject : public Object
{
public:
	MapObject() {}
	virtual ~MapObject() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;

public:
	static MapObject* Factory(MAP_OBJECT_TYPE type);
};

