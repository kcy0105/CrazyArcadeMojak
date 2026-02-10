#include "pch.h"
#include "Monster.h"
#include "TimeManager.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Collider.h"

Monster::Monster()
{
	SetObjectType(ObjectType::Monster);
	AddComponent<BoxCollider>()->SetSize({ 50, 50 });
}

Monster::~Monster()
{
}

void Monster::Init()
{
	__super::Init();
}

void Monster::Update()
{
	__super::Update();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_pos += _dir * _speed * deltaTime;
}

void Monster::Render(HDC hdc)
{
	__super::Render(hdc);
	Utils::DrawRect(hdc, _pos, static_cast<int>(_size), static_cast<int>(_size));
}

void Monster::Release()
{
	__super::Release();
}

void Monster::OnColliderBeginOverlap(Collider* collider, Collider* other)
{
	if (other->GetOwner()->GetObjectType() == ObjectType::Wall)
	{
		_dir *= -1;
	}
}

void Monster::OnColliderEndOverlap(Collider* collider, Collider* other)
{
}
