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

	virtual void OnColliderStayOverlap(Collider* collider, Collider* other) override;

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

	uint64 GetObjectId() const { return _objectId; }
	PLAYER_STATE GetState() const { return _state; }
	DIR GetDir() const { return _dir; }
	Vec2Int GetTilePos() const { return Utils::WorldToTile(_pos); }

protected:
	uint64 _objectId = {};
	PLAYER_STATE _state = {};
	DIR _dir = {};
	float _colSize = {};
	float _moveSpeed = {};

	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};

	FlipbookRenderer* _fb = {};

	bool _keyPressed = false;
};

