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
		SetDir(DIR_LEFT);
		SetState(PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Right))
	{
		SetDir(DIR_RIGHT);
		SetState(PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Up))
	{
		SetDir(DIR_UP);
		SetState(PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Down))
	{
		SetDir(DIR_DOWN);
		SetState(PLAYER_STATE_MOVE);
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
	__super::OnUpdateIdle();

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
		SetState(PLAYER_STATE_IDLE);
		_dirtyFlag = true;
	}
}

void MyPlayer::SyncToServer()
{
	if (_dirtyFlag == false)
		return;

	int state = GetState();

	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}
