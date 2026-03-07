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
	AddComponent<BoxCollider>()->SetSize({ 30, 30 });

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

void Player::OnRelease()
{

}

void Player::OnColliderStayOverlap(Collider* collider, Collider* other)
{
	auto bomb = dynamic_cast<WaterBomb*>(other->GetOwner());
	if (bomb && bomb->GetOwner() == this && bomb->GetOwnerCanPass())
		return;

	BlockingObject* block = dynamic_cast<BlockingObject*>(other->GetOwner());

	if (block)
	{
		BoxCollider* myBox = static_cast<BoxCollider*>(collider);
		BoxCollider* blockBox = static_cast<BoxCollider*>(other);

		RECT r1 = myBox->GetRect();
		RECT r2 = blockBox->GetRect();

		RECT intersect = {};
		if (::IntersectRect(&intersect, &r1, &r2))
		{
			int32 w = intersect.right - intersect.left;
			int32 h = intersect.bottom - intersect.top;

			if (w > h)
			{
				if (intersect.top == r2.top)
				{
					_pos.y -= h;
				}
				else
				{
					_pos.y += h;
				}
			}
			else
			{
				if (intersect.left == r2.left)
				{
					_pos.x -= w;
				}
				else
				{
					_pos.x += w;
				}
			}
		}
	}
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
}

