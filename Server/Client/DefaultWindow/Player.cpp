#include "pch.h"
#include "Player.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "FlipbookRenderer.h"
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

	_fb = AddComponent<FlipbookRenderer>();

	SetDir(DIR_DOWN);
	SetState(PLAYER_STATE_IDLE);
}

void Player::OnUpdate()
{
	//float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	//Vec2 dir = _destPos - _pos;

	//if (dir.LengthSquared() > 1)
	//{
	//	dir.Normalize();

	//	_pos += dir * _moveSpeed * deltaTime;
	//}

	switch (_state)
	{
	case PLAYER_STATE_IDLE:
		OnUpdateIdle();
		break;
	case PLAYER_STATE_MOVE:
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
	case PLAYER_STATE_IDLE:
		_fb->SetFlipbook(_flipbookIdle[_dir]);
		break;
	case PLAYER_STATE_MOVE:
		_fb->SetFlipbook(_flipbookMove[_dir]);
		break;
	}
}

void Player::SetState(PLAYER_STATE state)
{
	if (_state == state)
		return;

	_state = state;

	UpdateAnimation();
}

void Player::SetDir(DIR dir)
{
	_dir = dir;
	UpdateAnimation();
}

void Player::OnUpdateIdle()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 dir = _destPos - _pos;

	if (dir.LengthSquared() > 1)
	{
		dir.Normalize();

		_pos += dir * 100 * deltaTime;
	}
}

void Player::OnUpdateMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	float correctionSpeed = 100;

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

	if (_dir == DIR_UP || _dir == DIR_DOWN)
	{
		float dir = _destPos.x - _pos.x;

		if (dir > 1)
		{
			dir /= ::abs(dir); // normalize

			_pos.x += dir * correctionSpeed * deltaTime;
		}
	}
	else
	{
		float dir = _destPos.y - _pos.y;

		if (dir > 1)
		{
			dir /= ::abs(dir); // normalize

			_pos.y += dir * correctionSpeed * deltaTime;
		}
	}
}

