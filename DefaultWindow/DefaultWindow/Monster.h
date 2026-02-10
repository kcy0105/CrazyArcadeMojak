#pragma once
#include "Object.h"


class Monster :public Object
{
public:
	Monster();
	virtual ~Monster() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

public:
	virtual void OnColliderBeginOverlap(Collider* collider, Collider* other) override;
	virtual void OnColliderEndOverlap(Collider* collider, Collider* other) override;

private:
	Vec2 _dir = { 1, 0 };
	float _speed = 100;
	float _size = 50;
};

