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

void FlipbookRenderer::SetFlipbook(Flipbook* flipbook)
{
	_flipbooks.clear();

	if (flipbook)
		_flipbooks.push_back(flipbook);

	_fbIndex = 0;
	_idx = 0;
	_sumTime = 0.f;
}

void FlipbookRenderer::SetFlipbook(const vector<Flipbook*>& flipbooks)
{
	_flipbooks = flipbooks;

	_fbIndex = 0;
	_idx = 0;
	_sumTime = 0.f;
}

Flipbook* FlipbookRenderer::GetCurrentFlipbook()
{
	if (_flipbooks.empty())
		return nullptr;

	return _flipbooks[_fbIndex];
}

void FlipbookRenderer::Update()
{
	Flipbook* fb = GetCurrentFlipbook();
	if (fb == nullptr)
		return;

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_sumTime += deltaTime;

	int32 frameCount = (fb->GetEnd() - fb->GetStart() + 1);
	float delta = fb->GetDuration() / frameCount;

	if (_sumTime >= delta)
	{
		_sumTime = 0.f;
		_idx++;

		// 마지막 프레임 도달
		if (_idx >= frameCount)
		{
			if (fb->GetLoop())
			{
				_idx = 0;
			}
			else
			{
				// 다음 Flipbook으로 이동
				if (_fbIndex + 1 < (int32)_flipbooks.size())
				{
					_fbIndex++;
					_idx = 0;
				}
				else
				{
					// 마지막 Flipbook이면 멈춤
					_idx = frameCount - 1;
				}
			}
		}
	}
}

void FlipbookRenderer::LateUpdate()
{
}

void FlipbookRenderer::Render(HDC hdc)
{
	Flipbook* fb = GetCurrentFlipbook();
	if (fb == nullptr)
		return;

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	Vec2 pos = GetOwner()->GetPos();

	::TransparentBlt(hdc,
		(int32)pos.x - fb->GetPivot().x - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)pos.y - fb->GetPivot().y - ((int32)cameraPos.y - GWinSizeY / 2),
		fb->GetSize().x,
		fb->GetSize().y,
		fb->GetTexture()->GetDC(),
		(fb->GetStart() + _idx) * fb->GetSize().x,
		fb->GetLine() * fb->GetSize().y,
		fb->GetSize().x,
		fb->GetSize().y,
		fb->GetTexture()->GetTransparent());
}

void FlipbookRenderer::Release()
{
}