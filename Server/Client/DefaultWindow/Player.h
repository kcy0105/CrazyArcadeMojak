#pragma once
#include "SyncObject.h"

class Component;
class Flipbook;
class FlipbookRenderer;
class WaterBomb;

class Player : public SyncObject
{
public:
	Player() {}
	virtual ~Player() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnDebugRender(HDC hdc) override;
	virtual void OnRelease() override;

protected:
	void UpdateAnimation();

	virtual void OnUpdateIdle();
	virtual void OnUpdateMove();

public:
	void SetState(PLAYER_STATE state);
	void SetDir(DIR dir);
	void SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }
	void SetServerPos(Pos serverPos) { _serverPos = serverPos; }
	void SetServerState(PLAYER_STATE serverState) { _serverState = serverState; }

	PLAYER_STATE GetState()			const { return _state; }
	DIR GetDir()					const { return _dir; }
	Vec2Int GetTilePos()			const { return Utils::WorldToTile(_pos); }
	RECT GetRect()					const 
	{
		return {	(LONG)_pos.x - PLAYER_SIZE / 2,
					(LONG)_pos.y - PLAYER_SIZE / 2,
				    (LONG)_pos.x + PLAYER_SIZE / 2 ,
				    (LONG)_pos.y + PLAYER_SIZE / 2 };
	}

protected:
	PLAYER_STATE _state = {};
	DIR _dir = {};
	float _moveSpeed = {};

	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};

	FlipbookRenderer* _fb = {};

	Pos _serverPos = {};
	PLAYER_STATE _serverState = {};
};

