#include "pch.h"
#include "TilemapEditor.h"
#include "ResourceManager.h"
#include "Tilemap.h"
#include "InputManager.h"
#include "Sprite.h"

void TilemapEditor::OnInit()
{
	_tilemap = GET_SINGLE(ResourceManager)->CreateTilemap(L"Tilemap");
	_tilemap->SetMapSize({ 18, 15 });
}

void TilemapEditor::OnUpdate()
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

		Pos worldPos = { (float)mousePos.x + screenX, (float)mousePos.y + screenY };

		Tile* tile = _tilemap->GetTileAt(Utils::WorldToTile(worldPos));
		if (tile)
		{
			tile->value = _currentBlockId;
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::S))
	{
		GET_SINGLE(ResourceManager)->SaveTilemap(L"Tilemap", L"Tilemap.txt");
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::A))
	{
		_tilemap = GET_SINGLE(ResourceManager)->LoadTilemap(L"Tilemap", L"Tilemap.txt");
	}
}

void TilemapEditor::OnRender(HDC hdc)
{
	if (_tilemap == nullptr)
		return;

	const Vec2Int mapSize = _tilemap->GetMapSize();

	vector<vector<Tile>>& tiles = _tilemap->GetTiles();

	Sprite* blockSpr = GET_SINGLE(ResourceManager)->GetSprite(L"SP_Block");
	Sprite* wallSpr = GET_SINGLE(ResourceManager)->GetSprite(L"SP_Wall");

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	// 컬링 : 보여야 할 애들만 보여주기
	int32 leftX = ((int32)cameraPos.x - GWinSizeX / 2);
	int32 leftY = ((int32)cameraPos.y - GWinSizeY / 2);
	int32 rightX = ((int32)cameraPos.x + GWinSizeX / 2);
	int32 rightY = ((int32)cameraPos.y + GWinSizeY / 2);

	int32 startX = (leftX - (int32)_pos.x)	/ TILE_SIZE;
	int32 startY = (leftY - (int32)_pos.y)	/ TILE_SIZE;
	int32 endX = (rightX - (int32)_pos.x)	/ TILE_SIZE;
	int32 endY = (rightY - (int32)_pos.y)	/ TILE_SIZE;

	vector<Sprite*> sprites = { nullptr, blockSpr, wallSpr };

	for (int32 y = startY; y <= endY; y++)
	{
		for (int32 x = startX; x <= endX; x++)
		{
			if (x < 0 || x >= mapSize.x)
				continue;
			if (y < 0 || y >= mapSize.y)
				continue;
			// 왼쪽 상단 모서리를 기준으로 맞추자

			if (tiles[y][x].value > 0)
			{
				Sprite* sprite = sprites[tiles[y][x].value];
				::TransparentBlt(hdc,
					(int32)_pos.x + x * TILE_SIZE - (sprite->GetSize().x - TILE_SIZE) / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
					(int32)_pos.y + y * TILE_SIZE - (sprite->GetSize().y - TILE_SIZE) - ((int32)cameraPos.y - GWinSizeY / 2),
					sprite->GetSize().x,
					sprite->GetSize().y,
					sprite->GetDC(),
					sprite->GetPos().x,
					sprite->GetPos().y,
					sprite->GetSize().x,
					sprite->GetSize().y,
					sprite->GetTransparent());
			}
			else
			{
				//Utils::DrawRectInWorld(hdc,
				//	{ _pos.x + x * tileSize + tileSize / 2, _pos.y + y * tileSize + tileSize / 2 },
				//	tileSize, tileSize);
			}
		}
	}
}

void TilemapEditor::OnRelease()
{
}
