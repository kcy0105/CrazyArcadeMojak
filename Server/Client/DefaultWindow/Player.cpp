#include "pch.h"
#include "Player.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "FlipbookComponent.h"
#include "ResourceManager.h"


void Player::OnInit()
{
	SetTag(L"Player");

	_flipbookIdle[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleUp");
	_flipbookIdle[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleDown");
	_flipbookIdle[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleLeft");
	_flipbookIdle[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleRight");

	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveRight");

	_fb = AddComponent<FlipbookComponent>();

	SetDir(DIR_DOWN);
	SetState(PlayerState::IDLE);
}

void Player::OnUpdate()
{
	switch (_state)
	{
	case PlayerState::IDLE:
		OnUpdateIdle();
		break;
	case PlayerState::MOVE:
		OnUpdateMove();
		break;
	}
}

void Player::OnRender(HDC hdc)
{
	

}

void Player::OnRelease()
{

}


void Player::UpdateAnimation()
{
	switch (_state)
	{
	case PlayerState::IDLE:
		_fb->SetFlipbook(_flipbookIdle[_dir]);
		break;
	case PlayerState::MOVE:
		_fb->SetFlipbook(_flipbookMove[_dir]);
		break;
	}
}

void Player::SetState(PlayerState state)
{
	if (_state == state)
		return;

	_state = state;
	UpdateAnimation();
}

void Player::SetDir(Dir dir)
{
	_dir = dir;
	UpdateAnimation();
}

void Player::OnUpdateIdle()
{
	MoveInput();
}

void Player::OnUpdateMove()
{
	MoveInput();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	switch (_dir)
	{
	case DIR_LEFT:
		_pos.x -= _moveSpeed * deltaTime;
		break;
	case DIR_RIGHT:
		_pos.x += _moveSpeed * deltaTime;
		break;
	case DIR_UP:
		_pos.y -= _moveSpeed * deltaTime;
		break;
	case DIR_DOWN:
		_pos.y += _moveSpeed * deltaTime;
		break;
	}

	if (!GET_SINGLE(InputManager)->GetButton(KeyType::Left)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Right)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Up)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Down))
	{
		SetState(PlayerState::IDLE);
	}
}

void Player::MoveInput()
{
	if (GET_SINGLE(InputManager)->GetButton(KeyType::Left))
	{
		SetDir(DIR_LEFT);
		SetState(PlayerState::MOVE);
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::Right))
	{
		SetDir(DIR_RIGHT);
		SetState(PlayerState::MOVE);
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::Up))
	{
		SetDir(DIR_UP);
		SetState(PlayerState::MOVE);
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::Down))
	{
		SetDir(DIR_DOWN);
		SetState(PlayerState::MOVE);
	}
}


