#include "pch.h"
#include "BubbleItem.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

void BubbleItem::OnInit()
{
	__super::OnInit();

	_spr->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"SP_BubbleItem"));
}
