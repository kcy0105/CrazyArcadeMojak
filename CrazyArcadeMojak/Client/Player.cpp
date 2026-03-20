#include "pch.h"
#include "Player.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "FlipbookRenderer.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "MapObject.h"
#include "CollisionManager.h"
#include "WaterBomb.h"
#include "ObjectManager.h"


void Player::OnInit()
{
	SetTag(L"Player");

	_flipbookIdle[DIR_UP]		= GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleUp");
	_flipbookIdle[DIR_DOWN]		= GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleDown");
	_flipbookIdle[DIR_LEFT]		= GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleLeft");
	_flipbookIdle[DIR_RIGHT]	= GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleRight");

	_flipbookMove[DIR_UP]		= GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveUp");
	_flipbookMove[DIR_DOWN]		= GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveDown");
	_flipbookMove[DIR_LEFT]		= GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveLeft");
	_flipbookMove[DIR_RIGHT]	= GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveRight");

	_fb = AddComponent<FlipbookRenderer>();
}

void Player::OnUpdate()
{
	switch (_mainState)
	{
	case PLAYER_STATE_NORMAL:
		OnUpdateNormal();
		break;
	case PLAYER_STATE_TRAPPED:
		OnUpdateTrapped();
		break;
	case PLAYER_STATE_DEAD:
		OnUpdateDead();
		break;
	case PLAYER_STATE_ESCAPE:
		OnUpdateEscape();
		break;
	}
}

void Player::OnRender(HDC hdc)
{
	//Utils::DrawTextInWorld(hdc, _pos, std::format(L"main: {} move: {}", (int32)_mainState, (int32)_moveState));
}

void Player::OnDebugRender(HDC hdc)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);

	HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

	Utils::DrawRectInWorld(hdc, GetPos(), PLAYER_SIZE, PLAYER_SIZE);

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);

	DeleteObject(pen);
}


void Player::UpdateAnimation_Main()
{
	switch (_mainState)
	{
	case PLAYER_STATE_NORMAL:
		break;
	case PLAYER_STATE_TRAPPED:
		_fb->SetFlipbook({ GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_PlayerTrap"), GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_PlayerBubble") });
		break;
	case PLAYER_STATE_DEAD:
		_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_PlayerDead"));
		break;
	case PLAYER_STATE_ESCAPE:
		_fb->SetFlipbook((GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_PlayerEscape")));
		break;
	}

}

void Player::UpdateAnimation_Move()
{
	switch (_mainState)
	{
	case PLAYER_STATE_NORMAL:
		switch (_moveState)
		{
		case MOVE_STATE_IDLE:
			_fb->SetFlipbook(_flipbookIdle[_dir]);
			break;
		case MOVE_STATE_MOVE:
			_fb->SetFlipbook(_flipbookMove[_dir]);
			break;
		}
	}
}

void Player::OnUpdateNormal()
{
	SyncFromServer();
}

void Player::OnUpdateTrapped()
{
	SyncFromServer();
}

void Player::OnUpdateDead()
{
}

void Player::OnUpdateEscape()
{
}



void Player::SetMainState(PLAYER_STATE mainState)
{
	if (_mainState == mainState)
		return;

	_mainState = mainState;
	UpdateAnimation_Main();
}

void Player::SetMoveState(MOVE_STATE moveState)
{
	if (_moveState == moveState)
		return;

	_moveState = moveState;

	UpdateAnimation_Move();
}

void Player::SetDir(DIR dir)
{
	if (_dir == dir)
		return;

	_dir = dir;

	UpdateAnimation_Move();
}

void Player::SyncFromServer()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 pos = GetPos();
	Vec2 diff = _targetPos - pos;
	float dist = diff.Length();

	if (diff.Length() < 1.f || diff.Length() > 200.f) // 너무 멀어도 바로 싱크.
	{
		SetPos(_targetPos);
		return;
	}

	float t = 20.f * deltaTime;
	t = min(t, 1.f);
	pos += diff * t;

	SetPos(pos);
}



