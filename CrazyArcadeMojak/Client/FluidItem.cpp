#include "pch.h"
#include "FluidItem.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

void FluidItem::OnInit()
{
	__super::OnInit();

	_spr->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"SP_FluidItem"));

}
