#include "pch.h"
#include "FlipbookRenderer.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Object.h"
#include "Texture.h"

void FlipbookRenderer::Init()
{
}

void FlipbookRenderer::Update()
{
	if (_flipbook->GetLoop() == false && _idx == _flipbook->GetEnd())
		return;

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_sumTime += deltaTime;

	int32 frameCount = (_flipbook->GetEnd() - _flipbook->GetStart() + 1);
	float delta = _flipbook->GetDuration() / frameCount;

	if (_sumTime >= delta)
	{
		_sumTime = 0.f;
		_idx = (_idx + 1) % frameCount;
	}
}

void FlipbookRenderer::LateUpdate()
{
}

void FlipbookRenderer::Render(HDC hdc)
{
	if (_flipbook == nullptr)
		return;

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	Vec2 pos = GetOwner()->GetPos();

	::TransparentBlt(hdc,
		(int32)pos.x - _flipbook->GetSize().x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)pos.y - _flipbook->GetSize().y / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
		_flipbook->GetSize().x,
		_flipbook->GetSize().y,
		_flipbook->GetTexture()->GetDC(),
		(_flipbook->GetStart() + _idx) * _flipbook->GetSize().x,
		_flipbook->GetLine() * _flipbook->GetSize().y,
		_flipbook->GetSize().x,
		_flipbook->GetSize().y,
		_flipbook->GetTexture()->GetTransparent());
}

void FlipbookRenderer::Release()
{
}
