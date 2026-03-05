#include "pch.h"
#include "EditScene.h"
#include "Tilemap.h"
#include "ResourceManager.h"
#include "TilemapObject.h"
#include "InputManager.h"

void EditScene::OnInit()
{
	_tilemap = GET_SINGLE(ResourceManager)->CreateTilemap(L"Tilemap");
	_tilemap->SetMapSize({ 18, 15 });
	_tilemap->SetTileSize(40);

	_tilemapObj = Object::CreateObject<TilemapObject>();
	_tilemapObj->SetTilemap(_tilemap);

	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"Block", L"Block.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_Block", texture, { 0, 0 }, { 40, 47 }, { 20, 27 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture(L"Wall", L"Wall.bmp", RGB(255, 0, 255));
		GET_SINGLE(ResourceManager)->CreateSprite(L"SP_Wall", texture, { 0, 0 }, { 40, 60 }, { 20, 40 });
	}
}

void EditScene::OnUpdate()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_0))
	{
		_currentBlockId = 0;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_1))
	{
		_currentBlockId = 1;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_2))
	{
		_currentBlockId = 2;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
	{
		Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
		int32 screenX = (int32)cameraPos.x - GWinSizeX / 2;
		int32 screenY = (int32)cameraPos.y - GWinSizeY / 2;

		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
		int32 posX = mousePos.x + screenX;
		int32 posY = mousePos.y + screenY;

		int32 x = posX / _tilemap->GetTileSize();
		int32 y = posY / _tilemap->GetTileSize();;

		Tile* tile = _tilemap->GetTileAt({ x, y });
		if (tile)
		{
			tile->value = _currentBlockId;
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::S))
	{
		GET_SINGLE(ResourceManager)->SaveTilemap(L"Tilemap", L"Tilemap.txt");
		::MessageBox(g_hwnd, L"Message", L"Saved", NULL);
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::A))
	{
		_tilemap = GET_SINGLE(ResourceManager)->LoadTilemap(L"Tilemap", L"Tilemap.txt");
		_tilemapObj->SetTilemap(_tilemap);
		::MessageBox(g_hwnd, L"Loaded", L"Loaded", NULL);
	}
}

void EditScene::OnRelease()
{
}
