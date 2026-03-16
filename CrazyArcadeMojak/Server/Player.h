#pragma once
#include "Object.h"

class WaterBomb;

class Player : public Object, public enable_shared_from_this<Player>
{
public:
	Player() : Object(OBJECT_TYPE_PLAYER) {}
	virtual ~Player() {}

	virtual void Update() override;

public:
	GameSessionRef session;

public:
	void SetState(PLAYER_STATE state) { _state = state; }
	void SetDir(DIR dir) { _dir = dir; }
	void SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }
	void SetTrapped(bool trapped) { _trapped = trapped; }

	PLAYER_STATE GetState()		const { return _state; }
	DIR GetDir()				const { return _dir; }
	float GetMoveSpeed()		const { return _moveSpeed; }
	RECT GetRect()				const
	{
		return {	(LONG)_pos.x - PLAYER_SIZE / 2,
					(LONG)_pos.y - PLAYER_SIZE / 2,
					(LONG)_pos.x + PLAYER_SIZE / 2 ,
					(LONG)_pos.y + PLAYER_SIZE / 2 };
	}
	bool GetTrapped()			const { return _trapped; }

	void AddOverlapBomb(WaterBombRef bomb) { _overlapBombs.push_back(bomb); }

private:
	PLAYER_STATE _state = {};
	DIR _dir = {};
	float _moveSpeed = {};
	bool _trapped = false;

	vector<weak_ptr<WaterBomb>> _overlapBombs;
};

