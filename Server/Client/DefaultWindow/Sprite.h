#pragma once
#include "ResourceBase.h"

class Texture;

class Sprite : public ResourceBase
{
public:
	Sprite(Texture* texture, int32 x, int32 y, int32 cx, int32 cy);
	Sprite(Texture* texture, int32 x, int32 y, int32 cx, int32 cy, int32 pivotx, int32 pivoty);
	virtual ~Sprite() {}

	HDC		GetDC();
	int32	GetTransparent();
	Vec2Int GetPos() { return Vec2Int{ _x, _y }; }
	Vec2Int GetSize() { return Vec2Int{ _cx, _cy }; }
	Vec2Int GetPivot() { return Vec2Int{ _pivotx, _pivoty }; }

private:
	Texture* _texture = nullptr;
	int32 _x = 0;
	int32 _y = 0;
	int32 _cx = 0;
	int32 _cy = 0;
	int32 _pivotx = 0;
	int32 _pivoty = 0;
};

