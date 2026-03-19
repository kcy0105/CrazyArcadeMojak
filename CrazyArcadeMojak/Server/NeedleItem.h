#pragma once
#include "Item.h"
class NeedleItem : public Item
{
public:
	NeedleItem() : Item(ITEM_TYPE_NEEDLE) {}

public:
	virtual void OnGet(Player& player) override;
};

