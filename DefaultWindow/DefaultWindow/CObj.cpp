#include "pch.h"
#include "CObj.h"
#include "CComponent.h"

CObj::CObj()
{
}

CObj::~CObj()
{
	
}

void CObj::Initialize()
{
	//for (CComponent* component : m_vecComponents)
	//	component->Initialize();
}

void CObj::Update()
{
	for (CComponent* component : m_vecComponents)
		component->Update();
}

void CObj::Render(HDC hDC)
{
	for (CComponent* component : m_vecComponents)
		component->Render(hDC);
}

void CObj::Release()
{
	for (CComponent* component : m_vecComponents)
	{
		component->Release();
		SAFE_DELETE(component);
	}

	m_vecComponents.clear();
		
}

void CObj::RemoveComponent(CComponent* pComponent)
{
	auto it = std::remove(m_vecComponents.begin(), m_vecComponents.end(), pComponent);
	m_vecComponents.erase(it);

	pComponent->Release();
	SAFE_DELETE(pComponent);
}

void CObj::OnColliderBeginOverlap(CCollider* collider, CCollider* other)
{
}

void CObj::OnColliderEndOverlap(CCollider* collider, CCollider* other)
{
}

void CObj::DestroyObject(CObj* pObj)
{
	pObj->Destroy();
}
