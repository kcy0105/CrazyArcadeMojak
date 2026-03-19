#pragma once
#include "MapObject.h"

class Player;

class WaterBomb : public MapObject, public enable_shared_from_this<WaterBomb>
{
public:
	WaterBomb() : MapObject(MAP_OBJECT_TYPE_WATER_BOMB) {}
	virtual ~WaterBomb() {}

	virtual void Update() override;

public:
	void Explode();

public:
	void SetOwner(PlayerRef owner) { _owner = owner; }
	void SetExploded(bool exploded) { _exploded = exploded; }

	PlayerRef	GetOwner()		const { return _owner.lock(); }
	uint8		GetRange()		const;
	bool		GetExploded()	const { return _exploded; }

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

	float _explodeTimer = 0.f;

	bool _exploded = false;
};

