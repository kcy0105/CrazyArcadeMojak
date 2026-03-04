#include "pch.h"
#include "Player.h"
#include "GameRoom.h"


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

		SetPos(pos);

		{
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(GetObjectId(), GetState(), GetDir(), (int32)pos.x, (int32)pos.y);
			room->Broadcast(sendBuffer);
		}

		//if (room->CanGo((uint8)_colSize, { (int32)pos.x, (int32)pos.y }))
		//{
		//	SetPos(pos);
		//}
		//else
		//{
		//	// TODO
		//}
	}

		
		break;
	}
}
