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

protected:
	void UpdateAnimation_Main();
	void UpdateAnimation_Move();

	virtual void OnUpdateNormal();
	virtual void OnUpdateTrapped();
	virtual void OnUpdateDead();
	virtual void OnUpdateEscape();

	void SyncFromServer();

public:
	virtual void SetMainState(PLAYER_STATE mainState);
	void SetMoveState(MOVE_STATE moveState);
	void SetDir(DIR dir);
	
	void SetTargetPos(Pos targetPos) { _targetPos = targetPos; }

	PLAYER_STATE GetMainState()		const { return _mainState; }
	MOVE_STATE GetMoveState()		const { return _moveState; }
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
	PLAYER_STATE _mainState = {};
	MOVE_STATE _moveState = {};
	DIR _dir = {};
	

	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};

	FlipbookRenderer* _fb = {};

	Pos _targetPos = {};
};

