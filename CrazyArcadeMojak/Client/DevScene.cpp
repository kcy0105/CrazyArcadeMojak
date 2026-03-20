#include "pch.h"
#include "DevScene.h"
#include "ResourceManager.h"
#include "UI.h"
#include "Image.h"
#include "Object.h"
#include "Player.h"
#include "MyPlayer.h"
#include "MapObject.h"
#include "ObjectManager.h"

void DevScene::OnInit()
{
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerUp", L"PlayerUp.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleUp", texture, { 47, 56 }, { 23, 36 }, 0, 0, 0, 0.f, false);
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveUp", texture, { 47, 56 }, { 23, 36 }, 0, 5, 0, 0.8f, true);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerDown", L"PlayerDown.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleDown", texture, { 47, 56 }, { 23, 36 }, 0, 0, 0, 0.f, false);
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveDown", texture, { 47, 56 }, { 23, 36 }, 0, 5, 0, 0.8f, true);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerLeft", L"PlayerLeft.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleLeft", texture, { 47, 56 }, { 23, 36 }, 0, 0, 0, 0.f, false);
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveLeft", texture, { 47, 56 }, { 23, 36 }, 0, 5, 0, 0.8f, true);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerRight", L"PlayerRight.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleRight", texture, { 47, 56 }, { 23, 36 }, 0, 0, 0, 0.f, false);
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveRight", texture, { 47, 56 }, { 23, 36 }, 0, 5, 0, 0.8f, true);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerTrap", L"PlayerTrap.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_PlayerTrap", texture, { 88, 144 }, { 44, 80 }, 0, 12, 0, 0.3f, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerBubble", L"PlayerBubble.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_PlayerBubble", texture, { 88, 144 }, { 44, 80 }, 0, 6, 0, 0.7f, true);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerDead", L"PlayerDead.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_PlayerDead", texture, { 88, 144 }, { 44, 80 }, 0, 12, 0, 1.f, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerEscape", L"PlayerEscape.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_PlayerEscape", texture, { 88, 144 }, { 44, 80 }, 0, 4, 0, 0.3f, false);
	}


	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"Block", L"Block.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_Block", texture, { 0, 0 }, { 40, 47 }, { 20, 27 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"Wall", L"Wall.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_Wall", texture, { 0, 0 }, { 40, 60 }, { 20, 40 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"WaterBomb", L"WaterBomb.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_WaterBomb_Idle", texture, { 46, 46 }, 0, 3, 0, 0.5f, true);
	}

	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BoomCenter", L"BoomCenter.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BoomCenter", texture, { 40, 40 }, 0, 7, 0, FLOW_EFFECT_DURATION, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BoomUp", L"BoomUp.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BoomUp", texture, { 40, 40 }, 0, 7, 0, FLOW_EFFECT_DURATION, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BoomUpEnd", L"BoomUpEnd.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BoomUpEnd", texture, { 40, 40 }, 0, 7, 0, FLOW_EFFECT_DURATION, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BoomDown", L"BoomDown.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BoomDown", texture, { 40, 40 }, 0, 7, 0, FLOW_EFFECT_DURATION, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BoomDownEnd", L"BoomDownEnd.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BoomDownEnd", texture, { 40, 40 }, 0, 7, 0, FLOW_EFFECT_DURATION, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BoomLeft", L"BoomLeft.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BoomLeft", texture, { 40, 40 }, 0, 7, 0, FLOW_EFFECT_DURATION, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BoomLeftEnd", L"BoomLeftEnd.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BoomLeftEnd", texture, { 40, 40 }, 0, 7, 0, FLOW_EFFECT_DURATION, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BoomRight", L"BoomRight.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BoomRight", texture, { 40, 40 }, 0, 7, 0, FLOW_EFFECT_DURATION, false);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BoomRightEnd", L"BoomRightEnd.bmp", RGB(0, 0, 0));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BoomRightEnd", texture, { 40, 40 }, 0, 7, 0, FLOW_EFFECT_DURATION, false);
	}

	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"BubbleItem", L"BubbleItem.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_BubbleItem", texture, { 0, 0 }, {38, 38});
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"FluidItem", L"FluidItem.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_FluidItem", texture, { 0, 0 }, { 38, 38 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"RollerItem", L"RollerItem.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_RollerItem", texture, { 0, 0 }, { 38, 38 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"NeedleItem", L"NeedleItem.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_NeedleItem", texture, { 0, 0 }, { 38, 38 });
	}

}

void DevScene::OnUpdate()
{
}

void DevScene::OnRelease()
{
}