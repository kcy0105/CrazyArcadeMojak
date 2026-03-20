#include "pch.h"
#include "RollerItem.h"
#include "Player.h"
#include "GameRoom.h"

void RollerItem::OnGet(Player& player)
{
	player.AddRollerCount();

	Protocol::S_PlayerNormalSpeed pkt;
	pkt.set_objectid(player.GetObjectId());
	pkt.set_normalspeed(player.GetNormalSpeed());

	player.session->SendPacket(pkt);
}
