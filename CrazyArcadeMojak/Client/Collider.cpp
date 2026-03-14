#include "pch.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Object.h"
#include "CollisionManager.h"

void Collider::Init()
{
	GET_SINGLE(CollisionManager)->RegisterCollider(this);
}

void Collider::Update()
{
}

void Collider::Render(HDC hdc)
{
}

void Collider::Release()
{
	GET_SINGLE(CollisionManager)->UnregisterCollider(this);
}

bool Collider::CheckCollisionBox2Box(BoxCollider* b1, BoxCollider* b2)
{
	RECT r1 = b1->GetRect();
	RECT r2 = b2->GetRect();
	RECT intersect = {};

	return ::IntersectRect(&intersect, &r1, &r2);
}

bool Collider::CheckCollisionCircle2Box(CircleCollider* s1, BoxCollider* b2)
{
	Vec2 cPos = s1->GetOwner()->GetPos();
	float radius = s1->GetRadius();

	Vec2 bPos = b2->GetOwner()->GetPos();
	Vec2 bSize = b2->GetSize();

	// Box의 min / max
	float left = bPos.x - bSize.x / 2;
	float right = bPos.x + bSize.x / 2;
	float top = bPos.y - bSize.y / 2;
	float bottom = bPos.y + bSize.y / 2;

	// 원 중심을 박스 내부로 clamp
	float closestX = std::max(left, std::min(cPos.x, right));
	float closestY = std::max(top, std::min(cPos.y, bottom));

	// 가장 가까운 점과 원 중심 거리
	Vec2 closestPoint(closestX, closestY);
	Vec2 dir = cPos - closestPoint;

	return dir.Length() <= radius;
}

bool Collider::CheckCollisionCircle2Circle(CircleCollider* s1, CircleCollider* s2)
{
	Vec2 p1 = s1->GetOwner()->GetPos();
	float r1 = s1->GetRadius();

	Vec2 p2 = s2->GetOwner()->GetPos();
	float r2 = s2->GetRadius();

	Vec2 dir = p1 - p2;
	float dist = dir.Length();

	return dist <= r1 + r2;
}
