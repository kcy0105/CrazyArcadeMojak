#include "pch.h"
#include "Player.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "FlipbookComponent.h"
#include "ResourceManager.h"


void Player::OnInit()
{
	SetTag(L"Player");

	_flipbookIdle[Protocol::DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleUp");
	_flipbookIdle[Protocol::DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleDown");
	_flipbookIdle[Protocol::DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleLeft");
	_flipbookIdle[Protocol::DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleRight");

	_flipbookMove[Protocol::DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveUp");
	_flipbookMove[Protocol::DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveDown");
	_flipbookMove[Protocol::DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveLeft");
	_flipbookMove[Protocol::DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveRight");

	_fb = AddComponent<FlipbookComponent>();

	SetDir(Protocol::DIR_DOWN);
	SetState(Protocol::PLAYER_STATE_IDLE);
}

void Player::OnUpdate()
{
	switch (info.state())
	{
	case Protocol::PLAYER_STATE_IDLE:
		OnUpdateIdle();
		break;
	case Protocol::PLAYER_STATE_MOVE:
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

void Player::SetPos(Vec2 pos)
{
	__super::SetPos(pos);

	info.set_posx((int32)pos.x);
	info.set_posy((int32)pos.y);
}


void Player::UpdateAnimation()
{
	switch (info.state())
	{
	case Protocol::PLAYER_STATE_IDLE:
		_fb->SetFlipbook(_flipbookIdle[info.dir()]);
		break;
	case Protocol::PLAYER_STATE_MOVE:
		_fb->SetFlipbook(_flipbookMove[info.dir()]);
		break;
	}
}

void Player::SetState(PlayerState state)
{
	if (info.state() == state)
		return;

	info.set_state(state);
	UpdateAnimation();
}

void Player::SetDir(Dir dir)
{
	info.set_dir(dir);
	UpdateAnimation();
}

void Player::OnUpdateIdle()
{
}

void Player::OnUpdateMove()
{
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
}

