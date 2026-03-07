#pragma once
#include "Object.h"

class Tilemap;

class TilemapEditor : public Object
{
public:
	TilemapEditor() {}
	virtual ~TilemapEditor() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;

private:
	Tilemap* _tilemap = nullptr;
	uint8 _currentBlockId = 0;
};

