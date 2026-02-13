#include "pch.h"
#include "Player.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "OrbitShield.h"
#include "Bullet.h"
#include "ScrewMissile.h"

void Player::Init()
{
	__super::Init();

	SetTag(L"Player");
	_orbitShield = Object::CreateObject<OrbitShield>();
	_orbitShield->SetOwner(this);
}

void Player::Update()
{
	__super::Update();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	
#pragma region MOVE
	Vec2 moveDir = { 0, 0 };
	if (GET_SINGLE(InputManager)->GetButton(KeyType::W))
	{
		moveDir += Vec2(0, -1);
	}
	if (GET_SINGLE(InputManager)->GetButton(KeyType::A))
	{
		moveDir += Vec2(-1, 0);
	}
	if (GET_SINGLE(InputManager)->GetButton(KeyType::S))
	{
		moveDir += Vec2(0, 1);
	}
	if (GET_SINGLE(InputManager)->GetButton(KeyType::D))
	{
		moveDir += Vec2(1, 0);
	}

	moveDir.Normalize();
	_pos += moveDir * _moveSpeed * deltaTime;
#pragma endregion

#pragma region ROTATE BARREL
	if (GET_SINGLE(InputManager)->GetButton(KeyType::Left))
	{
		_barrelAngle += _barrelSpeed * deltaTime;
	}
	if (GET_SINGLE(InputManager)->GetButton(KeyType::Right))
	{
		_barrelAngle -= _barrelSpeed * deltaTime;
	}
#pragma endregion

#pragma region FIRE
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		Projectile* bullet = Object::CreateObject<Bullet>();
		bullet->SetPos(_pos);
		bullet->SetDirection(Utils::GetDirection(_barrelAngle));
		bullet->SetOwner(this);
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Q))
	{
		Projectile* bullet = Object::CreateObject<ScrewMissile>();
		bullet->SetPos(_pos);
		bullet->SetDirection(Utils::GetDirection(_barrelAngle));
		bullet->SetOwner(this);
	}


#pragma endregion
}

void Player::Render(HDC hdc)
{
	__super::Render(hdc);

	// Body
	Utils::DrawCircleInWorld(hdc, _pos, 25);

	// Barrel
	float w = 10;
	float h1 = 50;
	float h2 = 20;

	Vec2 dir = Utils::GetDirection(_barrelAngle);

	Vec2 m1 = _pos + h1 * dir;
	Vec2 m2 = _pos - h2 * dir;
	Vec2 a = m1 + w / 2 * Vec2(-dir.y, dir.x);
	Vec2 b = m1 - w / 2 * Vec2(-dir.y, dir.x);
	Vec2 c = m2 + w / 2 * Vec2(-dir.y, dir.x);
	Vec2 d = m2 - w / 2 * Vec2(-dir.y, dir.x);

	Utils::DrawLineInWorld(hdc, a, b);
	Utils::DrawLineInWorld(hdc, d, c);
	Utils::DrawLineInWorld(hdc, c, a);
	Utils::DrawLineInWorld(hdc, b, d);

	Utils::DrawCircleInWorld(hdc, _pos, 10);

}

void Player::Release()
{
	__super::Release();

	if (_orbitShield)
		Object::DestroyObject(_orbitShield);
}
