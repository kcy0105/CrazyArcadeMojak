#pragma once
#include "BlockingObject.h"

class Player;

class WaterBomb : public BlockingObject
{
public:
	WaterBomb() {}
	virtual ~WaterBomb() {}

public:
	void SetOwner(PlayerRef owner) { _owner = owner; }
	PlayerRef	GetOwner()			const { return _owner.lock(); }

public:
	virtual bool BlocksPlayer(const Player* player) const override
	{
		for (auto& w : _passablePlayers)
		{
			auto p = w.lock();
			if (p.get() == player)
				return false;
		}

		return true;
	}

	void AddPassablePlayer(PlayerRef player);
	void RemovePassablePlayer(PlayerRef player);

private:
	weak_ptr<Player> _owner = {};

	vector<weak_ptr<Player>> _passablePlayers;
};

