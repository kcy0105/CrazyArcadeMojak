#include "pch.h"
#include "WaterBomb.h"
#include "GameRoom.h"

void WaterBomb::Update()
{
    _explodeTimer += TICK;

    if (_explodeTimer >= EXPLOSION_TIME)
    {
        // TODO : 반경 체크. Player. WaterBomb.
        
        // TEMP : 터진다.
        Protocol::S_Explode pkt;
        pkt.set_objectid(GetObjectId());

        room->Broadcast(pkt);
        Destroy();
    }
}

void WaterBomb::AddPassablePlayer(PlayerRef player)
{
    _passablePlayers.push_back(player);
}

void WaterBomb::RemovePassablePlayer(PlayerRef player)
{
    for (auto it = _passablePlayers.begin(); it != _passablePlayers.end();)
    {
        auto p = it->lock();
        if (!p || p == player)
            it = _passablePlayers.erase(it);
        else
            ++it;
    }
}