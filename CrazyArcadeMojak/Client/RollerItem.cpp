#include "pch.h"
#include "RollerItem.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

void RollerItem::OnInit()
{
	__super::OnInit();

	_spr->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"SP_RollerItem"));
}
