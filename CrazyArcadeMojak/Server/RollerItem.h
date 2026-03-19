#pragma once
#include "Item.h"
class RollerItem : public Item
{
public:
	RollerItem() : Item(ITEM_TYPE_ROLLER) {}

public:
	virtual void OnGet(Player& player) override;
};

