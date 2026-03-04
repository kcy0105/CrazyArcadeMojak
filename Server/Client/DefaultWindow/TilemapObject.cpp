#include "pch.h"
#include "TilemapObject.h"
#include "Tilemap.h"
#include "ResourceManager.h"
#include "Sprite.h"

void TilemapObject::OnInit()
{

}

void TilemapObject::OnUpdate()
{

}

void TilemapObject::OnRender(HDC hdc)
{
	if (_tilemap == nullptr)
		return;


	const Vec2Int mapSize = _tilemap->GetMapSize();
	const int32 tileSize = _tilemap->GetTileSize();

	vector<vector<Tile>>& tiles = _tilemap->GetTiles();

	Sprite* blockSpr = GET_SINGLE(ResourceManager)->GetSprite(L"SP_Block");
	Sprite* wallSpr = GET_SINGLE(ResourceManager)->GetSprite(L"SP_Wall");

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	// 컬링 : 보여야 할 애들만 보여주기
	int32 leftX = ((int32)cameraPos.x - GWinSizeX / 2);
	int32 leftY = ((int32)cameraPos.y - GWinSizeY / 2);
	int32 rightX = ((int32)cameraPos.x + GWinSizeX / 2);
	int32 rightY = ((int32)cameraPos.y + GWinSizeY / 2);

	int32 startX = (leftX - (int32)_pos.x) / tileSize;
	int32 startY = (leftY - (int32)_pos.y) / tileSize;
	int32 endX = (rightX - (int32)_pos.x) / tileSize;
	int32 endY = (rightY - (int32)_pos.y) / tileSize;
	
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
					(int32)_pos.x + x * tileSize - (sprite->GetSize().x - tileSize)/2 - ((int32)cameraPos.x - GWinSizeX / 2),
					(int32)_pos.y + y * tileSize - (sprite->GetSize().y - tileSize) - ((int32)cameraPos.y - GWinSizeY / 2),
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

void TilemapObject::OnRelease()
{

}
