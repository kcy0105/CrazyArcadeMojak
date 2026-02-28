#include "pch.h"
#include "Camera.h"
#include "CameraComponent.h"
#include "Player.h"

void Camera::OnInit()
{
	AddComponent<CameraComponent>();
}

void Camera::OnUpdate()
{
}

void Camera::OnLateUpdate()
{
	_pos = _player->GetPos();
}

void Camera::OnRender(HDC hdc)
{
}

void Camera::OnRelease()
{
}
