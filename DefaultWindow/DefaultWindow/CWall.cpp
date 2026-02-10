#include "pch.h"
#include "CWall.h"
#include "CBoxCollider.h"

CWall::CWall()
{
	SetObjectType(ObjectType::Wall);

	m_pBoxCollider = AddComponent<CBoxCollider>();
}

CWall::~CWall()
{
}

void CWall::Initialize()
{
	__super::Initialize();
}

void CWall::Update()
{
	__super::Update();
}

void CWall::Render(HDC hDC)
{
	__super::Render(hDC);
	Utils::DrawRect(hDC, m_pos, static_cast<int>(m_size.x), static_cast<int>(m_size.y));
}

void CWall::Release()
{
	__super::Release();
}

void CWall::SetSize(Vec2 size)
{
	m_size = size;
	if (m_pBoxCollider)
		m_pBoxCollider->SetSize(size);
}