#include "pch.h"
#include "CMonster.h"
#include "CTimeManager.h"
#include "CBoxCollider.h"
#include "CCircleCollider.h"
#include "CCollider.h"

CMonster::CMonster()
{
	SetObjectType(ObjectType::Monster);
	AddComponent<CBoxCollider>()->SetSize({ 50, 50 });
}

CMonster::~CMonster()
{
}

void CMonster::Initialize()
{
	__super::Initialize();
}

void CMonster::Update()
{
	__super::Update();
	float deltaTime = GET_SINGLE(CTimeManager)->GetDeltaTime();

	m_pos += m_dir * m_fSpeed * deltaTime;
}

void CMonster::Render(HDC hDC)
{
	__super::Render(hDC);
	Utils::DrawRect(hDC, m_pos, static_cast<int>(m_fSize), static_cast<int>(m_fSize));
}

void CMonster::Release()
{
	__super::Release();
}

void CMonster::OnColliderBeginOverlap(CCollider* collider, CCollider* other)
{
	if (other->GetOwner()->GetObjectType() == ObjectType::Wall)
	{
		m_dir *= -1;
	}
}

void CMonster::OnColliderEndOverlap(CCollider* collider, CCollider* other)
{
}
