#pragma once
#include "Object.h"

class Player : public Object
{
public:
	Player() {}
	virtual ~Player() {}

	virtual void Update() override;

public:
	GameSessionRef session;

public:
	void SetState(PLAYER_STATE state) { _state = state; }
	PLAYER_STATE GetState() { return _state; }
	void SetDir(DIR dir) { _dir = dir; }
	DIR GetDir() { return _dir; }
	void SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }
	float GetMoveSpeed() { return _moveSpeed; }
	void SetColSize(float colSize) { _colSize = colSize; }
	float GetColSize() { return _colSize; }

private:
	PLAYER_STATE _state = {};
	DIR _dir = {};
	float _moveSpeed = {};
	float _colSize = 30;
};

