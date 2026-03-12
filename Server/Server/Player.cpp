#include "pch.h"
#include "Player.h"
#include "GameRoom.h"
#include "WaterBomb.h"

void Player::Update()
{
	switch (_state)
	{
	case PLAYER_STATE_IDLE:
		break;
	case PLAYER_STATE_MOVE:
	{
		Pos pos = GetPos();

		switch (_dir)
		{
		case DIR_LEFT:
			pos.x -= _moveSpeed * TICK;
			break;
		case DIR_RIGHT:
			pos.x += _moveSpeed * TICK;
			break;
		case DIR_UP:
			pos.y -= _moveSpeed * TICK;
			break;
		case DIR_DOWN:
			pos.y += _moveSpeed * TICK;
			break;
		}
		
		room->TryMove(*this, pos);
		//cout << "{ " << _pos.x << ", " << _pos.y << " }" << endl;

		for (auto it = _overlapBombs.begin(); it != _overlapBombs.end();)
		{
			WaterBombRef bomb = it->lock();

			if (!bomb)
			{
				it = _overlapBombs.erase(it);
				continue;
			}

			RECT r1 = GetRect();
			RECT r2 = bomb->GetRect();
			RECT r = {};

			if (::IntersectRect(&r, &r1, &r2) == false)
			{
				bomb->RemovePassablePlayer(shared_from_this());
				it = _overlapBombs.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

		break;
	}
}
