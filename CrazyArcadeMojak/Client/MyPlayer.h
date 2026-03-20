#pragma once
#include "Player.h"
class MyPlayer : public Player
{
public:
	MyPlayer() {}
	virtual ~MyPlayer() {}

private:
	virtual void OnUpdate() override;
	virtual void OnUpdateNormal() override;
	virtual void OnUpdateTrapped() override;
	virtual void OnUpdateDead() override;
	virtual void OnUpdateEscape() override;

	void HandleMoveInput_Idle();
	void HandleMoveInput_Move();
	void HandleBombInput();
	void HandleSkillInput();

	void Move();
	void CheckOverlapBombs();

public:
	virtual void SetMainState(PLAYER_STATE mainState) override;

	

public:
	void SpawnWaterBomb();
	void AddOverlapBomb(WaterBomb* bomb) { _overlapBombs.push_back(bomb); }

	float GetMoveSpeed()
	{
		switch (_mainState)
		{
		case PLAYER_STATE_NORMAL:
			return _normalSpeed;
		case PLAYER_STATE_TRAPPED:
			return MOVE_SPEED_TRAPPED;
		}

		return 0.f;
	}

	void SetNormalSpeed(float normalSpeed) { _normalSpeed = normalSpeed; }

private:
	vector<WaterBomb*> _overlapBombs;
	float _normalSpeed = MOVE_SPEED_NORMAL;

private:
	void TryMove(Pos nextPos);

private:
	void SyncToServer();
	
	bool _moveDirtyFlag = false;
};

