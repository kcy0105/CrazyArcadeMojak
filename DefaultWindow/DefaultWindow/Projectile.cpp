#include "pch.h"
#include "Projectile.h"
#include "TimeManager.h"
#include "CircleCollider.h"

void Projectile::Init()
{
	__super::Init();

	AddComponent<CircleCollider>()->SetRadius(_radius);
}

void Projectile::Update()
{
	__super::Update();
	
	UpdatePos();
	
	// 시간 경과하면 Destroy
}

void Projectile::Render(HDC hdc)
{
	__super::Render(hdc);
	Utils::DrawCircleInWorld(hdc, _pos, static_cast<int32>(_radius));
}

void Projectile::Release()
{
	__super::Release();
}

void Projectile::OnColliderBeginOverlap(Collider* collider, Collider* other)
{
	wstring tag = other->GetOwner()->GetTag();
	if (tag == L"Monster")
	{
		Destroy();
		Object::DestroyObject(other->GetOwner());
	}
	else if (tag == L"Wall")
	{
		Destroy();
	}
}

void Projectile::UpdatePos()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_pos += _speed * _dir * deltaTime;
}
