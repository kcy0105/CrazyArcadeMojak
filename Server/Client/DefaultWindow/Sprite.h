#pragma once
#include "ResourceBase.h"

class Texture;

class Sprite : public ResourceBase
{
public:
	Sprite(Texture* texture, Vec2Int pos, Vec2Int size);
	Sprite(Texture* texture, Vec2Int pos, Vec2Int size, Vec2Int pivot);
	virtual ~Sprite() {}

	HDC		GetDC();
	int32	GetTransparent();
	Vec2Int GetPos() { return _pos; }
	Vec2Int GetSize() { return _size; }
	Vec2Int GetPivot() { return _pivot; }

private:
	Texture* _texture = nullptr;
	Vec2Int _pos = {};
	Vec2Int _size = {};
	Vec2Int _pivot = {};
};

