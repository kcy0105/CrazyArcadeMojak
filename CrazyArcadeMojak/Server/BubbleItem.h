#pragma once
#include "Item.h"
class BubbleItem : public Item
{
public:
	BubbleItem() : Item(ITEM_TYPE_BUBBLE) {}

public:
	virtual void OnGet(Player& player) override;
};

