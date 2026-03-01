#pragma once
#include "Object.h"

class Component;
class Flipbook;
class FlipbookComponent;

class Player : public Object
{
public:
	Player() {}
	virtual ~Player() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;

	virtual void SetPos(Vec2 pos) override;

	void SetState(PlayerState state);
	void SetDir(Dir dir);


protected:
	void UpdateAnimation();

	virtual void OnUpdateIdle();
	virtual void OnUpdateMove();
	

protected:
	float _moveSpeed = 200.f;

	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};

	FlipbookComponent* _fb;

public:
	Protocol::ObjectInfo info;

public:
	uint64 GetId() { return info.objectid(); }
};

