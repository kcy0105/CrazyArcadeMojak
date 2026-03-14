#pragma once
#include "Player.h"
class MyPlayer : public Player
{
public:
	MyPlayer() {}
	virtual ~MyPlayer() {}

private:
	virtual void OnUpdate() override;
	virtual void OnUpdateIdle() override;
	virtual void OnUpdateMove() override;

	void HandleMoveInput_Idle();
	void HandleMoveInput_Move();
	void HandleBombInput();

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
};

