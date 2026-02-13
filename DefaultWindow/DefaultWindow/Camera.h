#pragma once
#include "Object.h"

class Player;

class Camera : public Object
{
public:
	Camera() {}
	virtual ~Camera() {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void SetPlayer(Player* player) { _player = player; }

private:
	Player* _player = {};
};

