#include "pch.h"
#include "FlowEffect.h"
#include "FlipbookRenderer.h"
#include "ResourceManager.h"
#include "TimeManager.h"

void FlowEffect::OnInit()
{
	_fb = AddComponent<FlipbookRenderer>();
	_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BoomCenter"));
}

void FlowEffect::OnUpdate()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_timer += deltaTime;

	if (_timer >= FLOW_EFFECT_DURATION)
	{
		Destroy();
	}
}

void FlowEffect::SetInfo(DIR dir, bool isEnd)
{
	switch (dir)
	{
	case DIR_UP:
		if (isEnd)
			_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BoomUpEnd"));
		else
			_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BoomUp"));
		break;
	case DIR_DOWN:
		if (isEnd)
			_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BoomDownEnd"));
		else
			_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BoomDown"));
		break;
	case DIR_LEFT:
		if (isEnd)
			_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BoomLeftEnd"));
		else
			_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BoomLeft"));
		break;
	case DIR_RIGHT:
		if (isEnd)
			_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BoomRightEnd"));
		else
			_fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BoomRight"));
		break;
	}

	int a = 3;
}
