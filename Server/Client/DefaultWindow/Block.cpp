#include "pch.h"
#include "Block.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

void Block::OnInit()
{
	SpriteRenderer* spr = AddComponent<SpriteRenderer>();
	spr->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"SP_Block"));
}

void Block::OnUpdate()
{
}

void Block::OnRender(HDC hdc)
{
}

void Block::OnRelease()
{
}
