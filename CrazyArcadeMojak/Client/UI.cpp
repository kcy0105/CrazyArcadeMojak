#include "pch.h"
#include "UI.h"
#include "InputManager.h"

RECT UI::GetRect()
{
	RECT rect =
	{
		(LONG)(_pos.x - _size.x / 2),
		(LONG)(_pos.y - _size.y / 2),
		(LONG)(_pos.x + _size.x / 2),
		(LONG)(_pos.y + _size.y / 2)
	};

	return rect;
}

bool UI::IsMouseInRect()
{
	RECT rect = GetRect();

	POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();

	return ::PtInRect(&rect, mousePos);
}

void UI::Init()
{
	OnInit();
}

void UI::Update()
{
	OnUpdate();
	for (UI* child : _children)
		child->Update();
}

void UI::Render(HDC hdc)
{
	OnRender(hdc);
	for (UI* child : _children)
		child->Render(hdc);
}

void UI::Release()
{
	OnRelease();
	for (UI* child : _children)
	{
		child->Release();
		SAFE_DELETE(child);
	}

	_children.clear();
}

void UI::AddChild(UI* ui)
{
	if (ui == nullptr)
		return;

	_children.push_back(ui);
}