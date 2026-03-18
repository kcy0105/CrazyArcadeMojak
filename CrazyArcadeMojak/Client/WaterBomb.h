#pragma once
#include "MapObject.h"

class Player;

class WaterBomb : public MapObject
{
public:
	WaterBomb() {}
	virtual ~WaterBomb() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;

public:
	void SetOwner(Player* owner)			{ _owner = owner; }
	void SetPassable(bool passable) { _passable = passable; }
	
	Player* GetOwner()				const	{ return _owner; }
	bool	GetOwnerPassable()		const	{ return _passable; }

public:
	virtual bool BlocksPlayer(const Player* player) const override
	{
		if (_passable)
			return false;

		return true;
	}

	void Explode(int32 up, int32 down, int32 left, int32 right);

private:
	Player* _owner = nullptr;
	bool _passable = true;
	uint64 _localId = 0;
};

