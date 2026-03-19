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
	void CheckGetItem();

public:
	GameSessionRef session;

public:
	void SetMainState(PLAYER_STATE mainState);
	void SetMoveState(MOVE_STATE moveState);
	void SetDir(DIR dir) { _dir = dir; }

	PLAYER_STATE	GetMainState()	const { return _mainState;	}
	MOVE_STATE		GetMoveState()	const { return _moveState;	}
	DIR				GetDir()		const { return _dir;		}
	RECT			GetRect()		const
	{
		return {	(LONG)_pos.x - PLAYER_SIZE / 2,
					(LONG)_pos.y - PLAYER_SIZE / 2,
					(LONG)_pos.x + PLAYER_SIZE / 2 ,
					(LONG)_pos.y + PLAYER_SIZE / 2 };
	}

	void AddOverlapBomb(WaterBombRef bomb) { _overlapBombs.push_back(bomb); }

	void AddBubbleCount() { _bubbleCount++; }
	void AddFluidCount() { _fluidCount++; }
	void AddRollerCount() { _rollerCount++; }

	uint8 GetBubbleCount() const { return _bubbleCount; }
	uint8 GetFluidCount() const { return _fluidCount; }
	uint8 GetRollerCount() const { return _rollerCount; }

	float GetMoveSpeed()
	{
		switch (_mainState)
		{
		case PLAYER_STATE_NORMAL:
			return GetNormalSpeed();
		case PLAYER_STATE_TRAPPED:
			return MOVE_SPEED_TRAPPED;
		}

		return 0.f;
	}

	float GetNormalSpeed() const { return  MOVE_SPEED_NORMAL + _rollerCount * 20.f;}
	
	void AddBombCount() { _bombCount++; }
	void SubBombCount() { _bombCount--; }

	bool CanSpawnBomb() const { return _bombCount < _bubbleCount; }

	void SetHasNeedle(bool hasNeedle) { _hasNeedle = hasNeedle; }

private:
	PLAYER_STATE _mainState = {};
	MOVE_STATE _moveState = {};

	DIR _dir = {};

	uint8 _bombCount = 0;
	
	uint8 _bubbleCount = 1;
	uint8 _fluidCount = 1;
	uint8 _rollerCount = 1;
	bool _hasNeedle = false;

	vector<weak_ptr<WaterBomb>> _overlapBombs;

	float _trapTimer = 0.f;
};

