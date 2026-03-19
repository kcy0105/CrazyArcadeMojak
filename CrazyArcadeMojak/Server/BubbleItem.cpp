#include "pch.h"
#include "BubbleItem.h"
#include "Player.h"

void BubbleItem::OnGet(Player& player)
{
	player.AddBubbleCount();
}
