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

	void HandleMoveInput_Idle();
	void HandleMoveInput_Move();
	void HandleBombInput();

	void Move();
	void CheckOverlapBombs();

public:
	virtual void SetMainState(PLAYER_STATE mainState) override;

	

public:
	bool SpawnWaterBomb();
	void AddOverlapBomb(WaterBomb* bomb) { _overlapBombs.push_back(bomb); }

public:
	queue<WaterBomb*> pendingBombs;

private:
	vector<WaterBomb*> _overlapBombs;

private:
	void TryMove(Pos nextPos);

private:
	void SyncToServer();
	
	bool _moveDirtyFlag = false;
	float _moveSpeed = {};
};

