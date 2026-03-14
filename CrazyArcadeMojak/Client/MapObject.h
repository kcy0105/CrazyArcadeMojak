#pragma once
#include "SyncObject.h"

class Player;

class MapObject : public SyncObject
{
public:
	MapObject() {}
	virtual ~MapObject() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnDebugRender(HDC hdc) override;
	virtual void OnRelease() override;

public:
	Vec2Int GetTilePos()	const { return Utils::WorldToTile(_pos); }
	RECT GetRect()			const
	{
		return {	(LONG)_pos.x - TILE_SIZE / 2,
					(LONG)_pos.y - TILE_SIZE / 2,
					(LONG)_pos.x + TILE_SIZE / 2 ,
					(LONG)_pos.y + TILE_SIZE / 2 };
	}

public:
	virtual bool BlocksPlayer(const Player* player) const { return true; }
};

