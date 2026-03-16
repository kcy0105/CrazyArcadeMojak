#include "pch.h"
#include "BreakableBlock.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "BoxCollider.h"

void BreakableBlock::OnInit()
{
	__super::OnInit();

	SpriteRenderer* spr = AddComponent<SpriteRenderer>();
	spr->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"SP_Block"));
}

void BreakableBlock::OnUpdate()
{
}

void BreakableBlock::OnRender(HDC hdc)
{
}

