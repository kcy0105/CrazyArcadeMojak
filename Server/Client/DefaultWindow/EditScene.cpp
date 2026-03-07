#include "pch.h"
#include "EditScene.h"
#include "Tilemap.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "TilemapEditor.h"

void EditScene::OnInit()
{
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"Block", L"Block.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_Block", texture, { 0, 0 }, { 40, 47 }, { 20, 27 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"Wall", L"Wall.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_Wall", texture, { 0, 0 }, { 40, 60 }, { 20, 40 });
	}

	Object::CreateObject<TilemapEditor>();
}

void EditScene::OnUpdate()
{
}

void EditScene::OnRelease()
{
}
