#include "pch.h"
#include "DevScene.h"
#include "ResourceManager.h"
#include "UI.h"
#include "Image.h"
#include "Object.h"
#include "Player.h"

void DevScene::OnInit()
{
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerUp", L"PlayerUp.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleUp", texture, { 47, 56 }, 0, 0, 0, 0.f, false);
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveUp", texture, {47, 56}, 0, 5, 0, 0.8f, true);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerDown", L"PlayerDown.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleDown", texture, { 47, 56 }, 0, 0, 0, 0.f, false);
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveDown", texture, { 47, 56 }, 0, 5, 0, 0.8f, true);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerLeft", L"PlayerLeft.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleLeft", texture, { 47, 56 }, 0, 0, 0, 0.f, false);
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveLeft", texture, { 47, 56 }, 0, 5, 0, 0.8f, true);
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerRight", L"PlayerRight.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleRight", texture, { 47, 56 }, 0, 0, 0, 0.f, false);
		GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveRight", texture, { 47, 56 }, 0, 5, 0, 0.8f, true);
	}
	



	Player* player = Object::CreateObject<Player>();
	player->SetPos({ 400, 300 });
}

void DevScene::OnUpdate()
{
}

void DevScene::OnRelease()
{
}
