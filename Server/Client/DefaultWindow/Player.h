#pragma once
#include "Object.h"

class Component;
class Flipbook;
class FlipbookRenderer;

class Player : public Object
{
public:
	Player() {}
	virtual ~Player() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;


protected:
	void UpdateAnimation();

	virtual void OnUpdateIdle();
	virtual void OnUpdateMove();

public:
	void SetObjectId(uint64 objectId) { _objectId = objectId; }
	void SetState(PLAYER_STATE state);
	void SetDir(DIR dir);
	void SetColSize(float colSize) { _colSize = colSize; }
	void SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }
	void SetDestPos(int x, int y) { _destPos = { (float)x, (float)y }; }
	void SetDestPos(float x, float y) { _destPos = { x, y }; }


	uint64 GetObjectId() { return _objectId; }
	PLAYER_STATE GetState() { return _state; }
	DIR GetDir() { return _dir; }

protected:
	uint64 _objectId = {};
	PLAYER_STATE _state = {};
	DIR _dir = {};
	float _colSize = {};
	float _moveSpeed = {};

	Pos _destPos = {};

	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};

	FlipbookRenderer* _fb = {};

};

