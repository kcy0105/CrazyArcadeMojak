#include "pch.h"
#include "SpriteComponent.h"
#include "Sprite.h"
#include "SceneManager.h"
#include "Object.h"

void SpriteComponent::Init()
{
}

void SpriteComponent::Update()
{
}

void SpriteComponent::LateUpdate()
{
}

void SpriteComponent::Render(HDC hdc)
{
	if (_sprite == nullptr)
		return;

	Vec2Int size = _sprite->GetSize();
	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	Vec2 pos = GetOwner()->GetPos();

	::TransparentBlt(hdc,
		(int32)pos.x - size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)pos.y - size.y / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		_sprite->GetTransparent());
}

void SpriteComponent::Release()
{
}
