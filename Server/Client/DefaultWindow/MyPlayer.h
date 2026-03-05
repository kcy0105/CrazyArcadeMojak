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

private:
	void SyncToServer();
	
	bool _dirtyFlag = false;
};

