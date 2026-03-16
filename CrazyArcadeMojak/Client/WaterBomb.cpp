#include "pch.h"
#include "WaterBomb.h"
#include "FlipbookRenderer.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "Player.h"

void WaterBomb::OnInit()
{
	__super::OnInit();

	FlipbookRenderer* fb = AddComponent<FlipbookRenderer>();
	fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_WaterBomb_Idle"));
}

void WaterBomb::OnUpdate()
{
}

void WaterBomb::OnRender(HDC hdc)
{
}

void WaterBomb::Explode()
{
	Destroy();
}



