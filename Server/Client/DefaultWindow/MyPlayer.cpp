#include "pch.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"

void MyPlayer::MoveInput()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Left))
	{
		SetDir(Protocol::DIR_LEFT);
		SetState(Protocol::PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Right))
	{
		SetDir(Protocol::DIR_RIGHT);
		SetState(Protocol::PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Up))
	{
		SetDir(Protocol::DIR_UP);
		SetState(Protocol::PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Down))
	{
		SetDir(Protocol::DIR_DOWN);
		SetState(Protocol::PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
}

void MyPlayer::OnUpdate()
{
	_dirtyFlag = false;

	__super::OnUpdate();

	SyncToServer();
}

void MyPlayer::OnUpdateIdle()
{
	MoveInput();
}

void MyPlayer::OnUpdateMove()
{
	__super::OnUpdateMove();

	MoveInput();

	if (!GET_SINGLE(InputManager)->GetButton(KeyType::Left)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Right)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Up)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Down))
	{
		SetState(Protocol::PLAYER_STATE_IDLE);
		_dirtyFlag = true;
	}
}

void MyPlayer::SyncToServer()
{
	if (_dirtyFlag == false)
		return;

	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}
