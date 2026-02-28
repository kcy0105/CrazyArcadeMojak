#include "pch.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "Object.h"

void CircleCollider::Init()
{
	__super::Init();
}

void CircleCollider::Update()
{
	__super::Update();
}

void CircleCollider::Render(HDC hdc)
{
	__super::Render(hdc);

	if (_showDebug == false)
		return;

	Vec2 pos = GetOwner()->GetPos();

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);

	HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

	Utils::DrawCircleInWorld(hdc, pos, (int)_radius);

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);

	DeleteObject(pen);
}

bool CircleCollider::CheckCollision(Collider* other)
{
	switch (other->GetColliderType())
	{
	case ColliderType::Box:
		return CheckCollisionCircle2Box(this, static_cast<BoxCollider*>(other));
	case ColliderType::Circle:
		return CheckCollisionCircle2Circle(this, static_cast<CircleCollider*>(other));
	}

	return false;
}
