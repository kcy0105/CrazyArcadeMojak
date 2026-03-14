#include "pch.h"
#include "CameraComponent.h"
#include "Object.h"

void CameraComponent::Init()
{
	__super::Init();
}

void CameraComponent::Update()
{
	__super::Update();
}

void CameraComponent::LateUpdate()
{
	__super::LateUpdate();

	if (_owner == nullptr)
		return;

	GET_SINGLE(SceneManager)->SetCameraPos(_owner->GetPos());
}

void CameraComponent::Render(HDC hdc)
{
	__super::Render(hdc);
}

void CameraComponent::Release()
{
	__super::Release();
}
