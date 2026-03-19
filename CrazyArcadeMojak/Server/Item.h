#pragma once
#include "MapObject.h"

class Player;

class Item : public MapObject
{
public:
	Item(ITEM_TYPE type) : MapObject(MAP_OBJECT_TYPE_ITEM), _type(type) {}
	virtual ~Item() {}

	virtual bool BlocksPlayer(const Player* player) const override { return false; }

	virtual void OnGet(Player& player) = 0;

	virtual RECT GetRect() override
	{
		return {	(LONG)_pos.x - ITEM_SIZE / 2,
					(LONG)_pos.y - ITEM_SIZE / 2,
					(LONG)_pos.x + ITEM_SIZE / 2 ,
					(LONG)_pos.y + ITEM_SIZE / 2 };
	}

public:
	ITEM_TYPE GetItemType() const { return _type; }

private:
	ITEM_TYPE _type;
};

