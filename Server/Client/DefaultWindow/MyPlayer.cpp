#include "pch.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"

void MyPlayer::OnUpdate()
{
	_dirtyFlag = false;

	__super::OnUpdate();

	if (_dirtyFlag)
		SyncToServer();
}

void MyPlayer::OnUpdateIdle()
{
	__super::OnUpdateIdle();

	if (GET_SINGLE(InputManager)->GetButton(KeyType::Left))
	{
		SetDir(DIR_LEFT);
		SetState(PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::Right))
	{
		SetDir(DIR_RIGHT);
		SetState(PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::Up))
	{
		SetDir(DIR_UP);
		SetState(PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::Down))
	{
		SetDir(DIR_DOWN);
		SetState(PLAYER_STATE_MOVE);
		_dirtyFlag = true;
	}
}

void MyPlayer::OnUpdateMove()
{
	__super::OnUpdateMove(); 

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
	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move(GetObjectId(), GetState(), GetDir(), GetPos().x, GetPos().y);
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}
