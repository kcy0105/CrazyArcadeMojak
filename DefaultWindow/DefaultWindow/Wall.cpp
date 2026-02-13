#include "pch.h"
#include "Wall.h"
#include "BoxCollider.h"

void Wall::Init()
{
	__super::Init();

	SetTag(L"Wall");
	_boxCollider = AddComponent<BoxCollider>();
}

void Wall::Update()
{
	__super::Update();
}

void Wall::Render(HDC hdc)
{
	__super::Render(hdc);
	Utils::DrawRectInWorld(hdc, _pos, static_cast<int>(_size.x), static_cast<int>(_size.y));
}

void Wall::Release()
{
	__super::Release();
}

void Wall::SetSize(Vec2 size)
{
	_size = size;
	if (_boxCollider)
		_boxCollider->SetSize(size);
}