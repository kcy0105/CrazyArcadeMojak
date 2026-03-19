#include "pch.h"
#include "NeedleItem.h"
#include "Player.h"

void NeedleItem::OnGet(Player& player)
{
	player.SetHasNeedle(true);
}
