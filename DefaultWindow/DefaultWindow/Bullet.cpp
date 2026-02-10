#include "pch.h"
#include "Bullet.h"
#include "TimeManager.h"
#include "CircleCollider.h"
#include "BoxCollider.h"

Bullet::Bullet()
{
	SetObjectType(ObjectType::Bullet);

	AddComponent<CircleCollider>()->SetRadius(_radius);
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	__super::Init();
}

void Bullet::Update()
{
	__super::Update();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_pos += _speed * _dir * deltaTime;

	if (_pos.x < 0 || _pos.x > GWinSizeX || _pos.y < 0 || _pos.y > GWinSizeY)
	{
		Destroy();
	}
}

void Bullet::Render(HDC hdc)
{
	__super::Render(hdc);
	Utils::DrawCircle(hdc, _pos, static_cast<int32>(_radius));
}

void Bullet::Release()
{
	__super::Release();
}

void Bullet::OnColliderBeginOverlap(Collider* collider, Collider* other)
{
	ObjectType colliderObjectType = other->GetOwner()->GetObjectType();
	if (colliderObjectType == ObjectType::Monster)
	{
		Destroy();
		Object::DestroyObject(other->GetOwner());
	}
	else if (colliderObjectType == ObjectType::Wall)
	{
		Destroy();
	}
	
}

void Bullet::OnColliderEndOverlap(Collider* collider, Collider* other)
{
}
