#pragma once
#include "Object.h"

class Player;

class Camera : public Object
{
public:
	Camera() {}
	virtual ~Camera() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;

	void SetPlayer(Player* player) { _player = player; }

private:
	Player* _player = {};
};

