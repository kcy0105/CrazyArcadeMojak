#pragma once
#include "Player.h"
class MyPlayer : public Player
{
public:
	MyPlayer() {}
	virtual ~MyPlayer() {}

private:
	void MoveInput();

	virtual void OnUpdate() override;
	virtual void OnUpdateIdle() override;
	virtual void OnUpdateMove() override;

	void SyncToServer();

protected:
	bool _dirtyFlag = false;
};

