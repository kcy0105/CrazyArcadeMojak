#include "pch.h"
#include "Player.h"
#include "GameRoom.h"
#include "WaterBomb.h"
#include "Item.h"

void Player::SetMainState(PLAYER_STATE mainState, bool broadcast)
{
	if (_mainState == mainState)
		return;

	_mainState = mainState;

	switch (mainState)
	{
	case PLAYER_STATE_NORMAL:
		break;
	case PLAYER_STATE_TRAPPED:
		_trapTimer = 0.f;
		break;
	case PLAYER_STATE_ESCAPE:
		_escapeTimer = 0.f;
		break;
	}

	if (broadcast)
	{
		Protocol::S_MainState pkt;
		pkt.set_playerid(GetObjectId());
		pkt.set_mainstate(mainState);

		room->Broadcast(pkt);
	}
}

void Player::SetMoveState(MOVE_STATE moveState)
{
	if (_moveState == moveState)
		return;

	_moveState = moveState;
}


void Player::Update()
{
	switch (_mainState)
	{
	case PLAYER_STATE_NORMAL:
		UpdateNormal();
		break;
	case PLAYER_STATE_TRAPPED:
		UpdateTrapped();
		break;
	case PLAYER_STATE_DEAD:
		UpdateDead();
		break;
	case PLAYER_STATE_ESCAPE:
		UpdateEscape();
		break;
	}
}

void Player::UpdateNormal()
{
	switch (_moveState)
	{
	case MOVE_STATE_IDLE:
		break;
	case MOVE_STATE_MOVE:
		Move();
		CheckOverlapBombs();
		break;
	}

	CheckKillOtherPlayer();
	CheckGetItem();
}

void Player::UpdateTrapped()
{
	switch (_moveState)
	{
	case MOVE_STATE_IDLE:
		break;
	case MOVE_STATE_MOVE:
		Move();
		break;
	}

	_trapTimer += TICK;

	if (_trapTimer >= DEAD_TIME)
	{
		SetMainState(PLAYER_STATE_DEAD, true);
	}
}

void Player::UpdateDead()
{
	
}

void Player::UpdateEscape()
{
	_escapeTimer += TICK;

	if (_escapeTimer >= 1.f)
	{
		SetMainState(PLAYER_STATE_NORMAL, true);
	}
}

void Player::Move()
{
	Pos pos = GetPos();

	switch (_dir)
	{
	case DIR_LEFT:
		pos.x -= GetMoveSpeed() * TICK;
		break;
	case DIR_RIGHT:
		pos.x += GetMoveSpeed() * TICK;
		break;
	case DIR_UP:
		pos.y -= GetMoveSpeed() * TICK;
		break;
	case DIR_DOWN:
		pos.y += GetMoveSpeed() * TICK;
		break;
	}

	room->TryMove(*this, pos);
}

void Player::CheckOverlapBombs()
{
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

void Player::CheckKillOtherPlayer()
{
	auto players = room->GetPlayers();

	for (auto& weak : players)
	{
		auto otherPlayer = weak.lock();
		if (!otherPlayer)
			continue;

		if (otherPlayer->GetObjectId() == GetObjectId())
			continue;
		
		if (otherPlayer->GetMainState() != PLAYER_STATE_TRAPPED)
			continue;

		RECT r1 = GetRect();
		RECT r2 = otherPlayer->GetRect();
		RECT r = {};

		if (::IntersectRect(&r, &r1, &r2))
		{
			otherPlayer->SetMainState(PLAYER_STATE_DEAD, true);
		}
	}
}

void Player::CheckGetItem()
{
	auto items = room->GetItems();

	for (auto& weak : items)
	{
		auto item = weak.lock();
		if (!item)
			continue;

		RECT r1 = GetRect();
		RECT r2 = item->GetRect();
		RECT r = {};

		if (::IntersectRect(&r, &r1, &r2))
		{
			item->OnGet(*this);

			Protocol::S_RemoveItem pkt;
			pkt.set_objectid(item->GetObjectId());
			room->Broadcast(pkt);

			item->Destroy();
		}
	}
}
