#include "pch.h"
#include "BoxCollider.h"
#include "Object.h"
#include "CircleCollider.h"

void BoxCollider::Init()
{
	__super::Init();
}

void BoxCollider::Update()
{
	__super::Update();
}

void BoxCollider::Render(HDC hdc)
{
	__super::Render(hdc);

	Vec2 pos = GetOwner()->GetPos();

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);

	HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

	Utils::DrawRectInWorld(hdc, pos, static_cast<int>(_size.x), static_cast<int>(_size.y));

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);

	DeleteObject(pen);
}

bool BoxCollider::CheckCollision(Collider* other)
{
	switch (other->GetColliderType())
	{
	case ColliderType::Box:
		return CheckCollisionBox2Box(this, static_cast<BoxCollider*>(other));
	case ColliderType::Circle:
		return CheckCollisionCircle2Box(static_cast<CircleCollider*>(other), this);
	}

	return false;
}
