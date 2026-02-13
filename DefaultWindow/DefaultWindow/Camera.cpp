#include "pch.h"
#include "Camera.h"
#include "CameraComponent.h"
#include "Player.h"

void Camera::Init()
{
	__super::Init();
	AddComponent<CameraComponent>();
}

void Camera::Update()
{
	__super::Update();

	_pos = _player->GetPos();
}

void Camera::Render(HDC hdc)
{
	__super::Render(hdc);
}

void Camera::Release()
{
	__super::Release();
}
