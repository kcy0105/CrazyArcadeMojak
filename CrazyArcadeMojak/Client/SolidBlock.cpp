#include "pch.h"
#include "SolidBlock.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

void SolidBlock::OnInit()
{
	__super::OnInit();

	SpriteRenderer* spr = AddComponent<SpriteRenderer>();
	spr->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"SP_Wall"));
}

void SolidBlock::OnUpdate()
{
}

void SolidBlock::OnRender(HDC hdc)
{
}

void SolidBlock::OnRelease()
{
}
