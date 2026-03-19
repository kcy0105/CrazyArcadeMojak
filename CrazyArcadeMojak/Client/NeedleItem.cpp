#include "pch.h"
#include "NeedleItem.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

void NeedleItem::OnInit()
{
	__super::OnInit();

	_spr->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"SP_NeedleItem"));
}
