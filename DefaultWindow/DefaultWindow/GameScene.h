#pragma once
#include "Scene.h"

class Player;

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Release() override;

private:
	Player* _player = {};
};

