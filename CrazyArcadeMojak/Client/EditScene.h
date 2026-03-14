#pragma once
#include "Scene.h"

class Tilemap;
class TilemapObject;

class EditScene : public Scene
{
public:
	EditScene() {}
	virtual ~EditScene() override {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRelease() override;

private:
	uint8 _currentBlockId = 0;
	Tilemap* _tilemap = nullptr;
	TilemapObject* _tilemapObj = nullptr;
};

