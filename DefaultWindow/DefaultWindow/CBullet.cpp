#include "pch.h"
#include "CBullet.h"
#include "CTimeManager.h"
#include "CCircleCollider.h"
#include "CBoxCollider.h"

CBullet::CBullet()
{
	SetObjectType(ObjectType::Bullet);

	AddComponent<CCircleCollider>()->SetRadius(m_fRadius);
}

CBullet::~CBullet()
{
}

void CBullet::Initialize()
{
	__super::Initialize();
}

void CBullet::Update()
{
	__super::Update();
	float deltaTime = GET_SINGLE(CTimeManager)->GetDeltaTime();

	m_pos += m_fSpeed * m_dir * deltaTime;

	if (m_pos.x < 0 || m_pos.x > GWinSizeX || m_pos.y < 0 || m_pos.y > GWinSizeY)
	{
		Destroy();
	}
}

void CBullet::Render(HDC hDC)
{
	__super::Render(hDC);
	Utils::DrawCircle(hDC, m_pos, static_cast<int32>(m_fRadius));
}

void CBullet::Release()
{
	__super::Release();
}

void CBullet::OnColliderBeginOverlap(CCollider* collider, CCollider* other)
{
	ObjectType colliderObjectType = other->GetOwner()->GetObjectType();
	if (colliderObjectType == ObjectType::Monster)
	{
		Destroy();
		CObj::DestroyObject(other->GetOwner());
	}
	else if (colliderObjectType == ObjectType::Wall)
	{
		Destroy();
	}
	
}

void CBullet::OnColliderEndOverlap(CCollider* collider, CCollider* other)
{
}
