#pragma once
#include "Scene.h"

class Player;

class GameScene : public Scene
{
public:
	GameScene() {}
	virtual ~GameScene() override {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRelease() override;

private:
	Player* _player = {};
};

