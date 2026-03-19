#include "pch.h"
#include "Item.h"
#include "SpriteRenderer.h"

void Item::OnInit()
{
	__super::OnInit();

	_spr = AddComponent<SpriteRenderer>();
}

void Item::OnDebugRender(HDC hdc)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);

	HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

	Utils::DrawRectInWorld(hdc, GetPos(), ITEM_SIZE, ITEM_SIZE);

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);

	DeleteObject(pen);
}