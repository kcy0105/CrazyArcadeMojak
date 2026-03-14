#include "pch.h"
#include "Projectile.h"
#include "TimeManager.h"
#include "CircleCollider.h"

void Projectile::OnInit()
{
	AddComponent<CircleCollider>()->SetRadius(_radius);
}

void Projectile::OnUpdate()
{
	UpdatePos();
	
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_timeSum += deltaTime;

	if (_timeSum > 3.f)
		Destroy();
}

void Projectile::OnRender(HDC hdc)
{
	Utils::DrawCircleInWorld(hdc, _pos, static_cast<int32>(_radius));
}

void Projectile::OnRelease()
{

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
