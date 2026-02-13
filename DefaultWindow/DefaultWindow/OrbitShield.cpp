#include "pch.h"
#include "OrbitShield.h"
#include "CircleCollider.h"
#include "TimeManager.h"

void OrbitShield::Init()
{
	__super::Init();

	SetTag(L"OrbitShield");
	AddComponent<CircleCollider>()->SetRadius(_radius);
}

void OrbitShield::Update()
{
	__super::Update();

	if (!_owner)
		return;

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	
	_angle += deltaTime * _speed;

	_pos = _owner->GetPos() + _orbitRadius * Utils::GetDirection(_angle);
}

void OrbitShield::Render(HDC hdc)
{
	__super::Render(hdc);

	if (!_owner)
		return;

	Utils::DrawCircleInWorld(hdc, _pos, static_cast<int32>(_radius));
}

void OrbitShield::Release()
{
	__super::Release();
}

void OrbitShield::OnColliderBeginOverlap(Collider* collider, Collider* other)
{
	if (other->GetOwner()->GetTag() == L"Monster")
	{
		Object::DestroyObject(other->GetOwner());
	}
}
