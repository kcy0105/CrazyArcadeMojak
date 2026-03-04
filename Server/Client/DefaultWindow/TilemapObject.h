#pragma once
#include "Object.h"

class Tilemap;

class TilemapObject : public Object
{
public:
	TilemapObject() {}
	virtual ~TilemapObject() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;

	void SetTilemap(Tilemap* tilemap) { _tilemap = tilemap; }
	Tilemap* GetTilemap() { return _tilemap; }

private:
	Tilemap* _tilemap = nullptr;
};

