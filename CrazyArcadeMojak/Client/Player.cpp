#include "pch.h"
#include "Player.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "FlipbookRenderer.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "BlockingObject.h"
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

	SetDir(DIR_DOWN);
	SetState(PLAYER_STATE_IDLE);
}

void Player::OnUpdate()
{
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

	//Utils::DrawTextInWorld(hdc, GetPos(), ::format(L"{0},{1}", GetPos().x, GetPos().y));
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
}

void Player::OnUpdateMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 pos = GetPos();
	Vec2 diff = _serverPos - pos;
	float dist = diff.Length();

	if (diff.Length() < 1.f || diff.Length() > 200.f) // 도착하면 상태 변경. 너무 멀어도 바로 싱크.
	{
		SetPos(_serverPos);
		SetState(_serverState);
		return;
	}

	pos += diff * 20.f * deltaTime;

	SetPos(pos);
}



