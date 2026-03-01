#include "pch.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "TimeManager.h"

void MyPlayer::MoveInput()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Left))
	{
		SetDir(Protocol::DIR_LEFT);
		SetState(Protocol::PLAYER_STATE_MOVE);

	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Right))
	{
		SetDir(Protocol::DIR_RIGHT);
		SetState(Protocol::PLAYER_STATE_MOVE);
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Up))
	{
		SetDir(Protocol::DIR_UP);
		SetState(Protocol::PLAYER_STATE_MOVE);
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Down))
	{
		SetDir(Protocol::DIR_DOWN);
		SetState(Protocol::PLAYER_STATE_MOVE);
	}
}

void MyPlayer::OnUpdateIdle()
{
	MoveInput();
}

void MyPlayer::OnUpdateMove()
{
	MoveInput();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	switch (info.dir())
	{
	case Protocol::DIR_LEFT:
		_pos.x -= _moveSpeed * deltaTime;
		break;
	case Protocol::DIR_RIGHT:
		_pos.x += _moveSpeed * deltaTime;
		break;
	case Protocol::DIR_UP:
		_pos.y -= _moveSpeed * deltaTime;
		break;
	case Protocol::DIR_DOWN:
		_pos.y += _moveSpeed * deltaTime;
		break;
	}

	if (!GET_SINGLE(InputManager)->GetButton(KeyType::Left)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Right)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Up)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Down))
	{
		SetState(Protocol::PLAYER_STATE_IDLE);
	}
}
