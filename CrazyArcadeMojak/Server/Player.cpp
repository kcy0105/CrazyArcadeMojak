#include "pch.h"
#include "Player.h"
#include "GameRoom.h"
#include "WaterBomb.h"

void Player::SetMainState(PLAYER_STATE mainState)
{
	if (_mainState == mainState)
		return;

	_mainState = mainState;

	switch (mainState)
	{
	case PLAYER_STATE_NORMAL:
		_moveSpeed = MOVE_SPEED_NORMAL;
		break;
	case PLAYER_STATE_TRAPPED:
		_moveSpeed = MOVE_SPEED_TRAPPED;
		_trapTimer = 0.f;
		break;
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
		SetMainState(PLAYER_STATE_DEAD);
		Protocol::S_Dead pkt;
		pkt.set_objectid(GetObjectId());
		room->Broadcast(pkt);
	}
}

void Player::UpdateDead()
{
	
}

void Player::Move()
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

	for (auto& p : players)
	{
		auto otherPlayer = p.lock();
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
			otherPlayer->SetMainState(PLAYER_STATE_DEAD);
			Protocol::S_Dead pkt;
			pkt.set_objectid(otherPlayer->GetObjectId());
			room->Broadcast(pkt);
		}
	}
}
