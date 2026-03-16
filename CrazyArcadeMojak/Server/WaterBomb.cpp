#include "pch.h"
#include "WaterBomb.h"
#include "GameRoom.h"

void WaterBomb::Update()
{
    if (_exploded) return;

    _explodeTimer += TICK;

    if (_explodeTimer >= EXPLOSION_TIME)
    {
        Explode();
    }
}

void WaterBomb::Explode()
{
    SetExploded(true);
    room->Explode(*this);
    Destroy();
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