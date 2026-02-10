#include "pch.h"
#include "Player.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Bullet.h"

Player::Player()
{
	SetObjectType(ObjectType::Player);
}

Player::~Player()
{
}

void Player::Init()
{
	__super::Init();
}

void Player::Update()
{
	__super::Update();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (GET_SINGLE(InputManager)->GetButton(KeyType::Up))
	{
		_pos += GetDirection() * deltaTime * 200;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		Bullet* bullet = Object::CreateObject<Bullet>();
		bullet->SetPos(_pos);
		bullet->SetDirection(GetDirection());
		bullet->SetOwner(this);
	}

	float angleSpeed = 100;
	if (GET_SINGLE(InputManager)->GetButton(KeyType::Left))
	{
		_angle += angleSpeed * deltaTime;
	}
	if (GET_SINGLE(InputManager)->GetButton(KeyType::Right))
	{
		_angle -= angleSpeed * deltaTime;
	}
}

void Player::Render(HDC hdc)
{
	__super::Render(hdc);

	// Body
	Utils::DrawCircle(hdc, _pos, 25);

	// Barrel
	float w = 10;
	float h1 = 50;
	float h2 = 20;

	Vec2 dir = GetDirection();

	Vec2 m1 = _pos + h1 * dir;
	Vec2 m2 = _pos - h2 * dir;
	Vec2 a = m1 + w / 2 * Vec2(-dir.y, dir.x);
	Vec2 b = m1 - w / 2 * Vec2(-dir.y, dir.x);
	Vec2 c = m2 + w / 2 * Vec2(-dir.y, dir.x);
	Vec2 d = m2 - w / 2 * Vec2(-dir.y, dir.x);

	Utils::DrawLine(hdc, a, b);
	Utils::DrawLine(hdc, b, d);
	Utils::DrawLine(hdc, d, c);
	Utils::DrawLine(hdc, c, a);

	Utils::DrawCircle(hdc, _pos, 10);

}

void Player::Release()
{
	__super::Release();
}
