#include "pch.h"
#include "Wall.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

void Wall::OnInit()
{
	SpriteRenderer* spr = AddComponent<SpriteRenderer>();
	spr->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"SP_Wall"));
}

void Wall::OnUpdate()
{
}

void Wall::OnRender(HDC hdc)
{
}

void Wall::OnRelease()
{
}
