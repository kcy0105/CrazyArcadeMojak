#include "pch.h"
#include "MapObject.h"
#include "ObjectManager.h"


void MapObject::OnInit()
{
}

void MapObject::OnUpdate()
{
}

void MapObject::OnRender(HDC hdc)
{
}

void MapObject::OnDebugRender(HDC hdc)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);

	HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

	Utils::DrawRectInWorld(hdc, GetPos(), TILE_SIZE, TILE_SIZE);

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);

	DeleteObject(pen);
}

void MapObject::OnRelease()
{
	__super::OnRelease();
	GET_SINGLE(ObjectManager)->CleartMapObjectAt(GetTilePos());
}