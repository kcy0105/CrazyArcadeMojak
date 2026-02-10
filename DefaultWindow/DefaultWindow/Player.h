#pragma once
#include "Object.h"

class Component;

class Player : public Object
{
public:
	Player();
	virtual ~Player() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

public:
	Vec2 GetDirection() { return Vec2(::cos(_angle * DEG2RAD), -::sin(_angle * DEG2RAD)); }

private:
	float _angle = 90;
};

