#include "pch.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "SceneManager.h"
#include "Object.h"

void SpriteRenderer::Init()
{
}

void SpriteRenderer::Update()
{
}

void SpriteRenderer::LateUpdate()
{
}

void SpriteRenderer::Render(HDC hdc)
{
	if (_sprite == nullptr)
		return;

	Vec2Int size = _sprite->GetSize();
	Vec2Int pivot = _sprite->GetPivot();
	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	Vec2 pos = GetOwner()->GetPos();

	::TransparentBlt(hdc,
		(int32)pos.x - pivot.x - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)pos.y - pivot.y - ((int32)cameraPos.y - GWinSizeY / 2),
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		_sprite->GetTransparent());
}

void SpriteRenderer::Release()
{
}
