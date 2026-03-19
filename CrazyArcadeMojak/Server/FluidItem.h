#pragma once
#include "Item.h"
class FluidItem : public Item
{
public:
	FluidItem() : Item(ITEM_TYPE_FLUID) {}

public:
	virtual void OnGet(Player& player) override;
};

