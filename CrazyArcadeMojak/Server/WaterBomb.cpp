#include "pch.h"
#include "WaterBomb.h"

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