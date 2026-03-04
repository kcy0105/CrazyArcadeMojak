#include "pch.h"
#include "Object.h"
#include "Player.h"

atomic<uint64> Object::s_idGenerator = 1;

PlayerRef Object::CreatePlayer()
{
	PlayerRef player = make_shared<Player>();
	player->SetObjectId(s_idGenerator++);
	player->SetObjectType(OBJECT_TYPE_PLAYER);

	return player;
}
