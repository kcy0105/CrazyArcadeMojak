#include "pch.h"
#include "FluidItem.h"
#include "Player.h"

void FluidItem::OnGet(Player& player)
{
	player.AddFluidCount();
}
