#include "pch.h"
#include "Object.h"
#include "Player.h"

atomic<uint64> Object::s_idGenerator = 1;

PlayerRef Object::CreatePlayer()
{
	PlayerRef player = make_shared<Player>();
	player->info.set_objectid(s_idGenerator++);
	player->info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);

	return player;
}
