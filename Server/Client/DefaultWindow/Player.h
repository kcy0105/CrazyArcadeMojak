#pragma once
#include "Object.h"

class Component;
class Flipbook;
class FlipbookComponent;

enum class PlayerState
{
	IDLE,
	MOVE,
};

class Player : public Object
{
public:
	Player() {}
	virtual ~Player() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;

private:
	void UpdateAnimation();
	void SetState(PlayerState state);
	void SetDir(Dir dir);

	void OnUpdateIdle();
	void OnUpdateMove();
	void MoveInput();

private:
	float _moveSpeed = 200.f;

	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};

	FlipbookComponent* _fb;

	PlayerState _state;
	Dir _dir;
};

