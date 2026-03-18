#pragma once
#include "Object.h"

class WaterBomb;

class Player : public Object, public enable_shared_from_this<Player>
{
public:
	Player() : Object(OBJECT_TYPE_PLAYER) {}
	virtual ~Player() {}

	virtual void Update() override;

	void UpdateNormal();
	void UpdateTrapped();
	void UpdateDead();

	void Move();
	void CheckOverlapBombs();
	void CheckKillOtherPlayer();

public:
	GameSessionRef session;

public:
	void SetMainState(PLAYER_STATE mainState);
	void SetMoveState(MOVE_STATE moveState);
	void SetDir(DIR dir) { _dir = dir; }
	void SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }

	PLAYER_STATE	GetMainState()	const { return _mainState;	}
	MOVE_STATE		GetMoveState()	const { return _moveState;	}
	DIR				GetDir()		const { return _dir;		}
	float			GetMoveSpeed()	const { return _moveSpeed;	}
	RECT			GetRect()		const
	{
		return {	(LONG)_pos.x - PLAYER_SIZE / 2,
					(LONG)_pos.y - PLAYER_SIZE / 2,
					(LONG)_pos.x + PLAYER_SIZE / 2 ,
					(LONG)_pos.y + PLAYER_SIZE / 2 };
	}

	void AddOverlapBomb(WaterBombRef bomb) { _overlapBombs.push_back(bomb); }

private:
	PLAYER_STATE _mainState = {};
	MOVE_STATE _moveState = {};

	DIR _dir = {};
	float _moveSpeed = MOVE_SPEED_NORMAL;

	vector<weak_ptr<WaterBomb>> _overlapBombs;

	float _trapTimer = 0.f;
};

