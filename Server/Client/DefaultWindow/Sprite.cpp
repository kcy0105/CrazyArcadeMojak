#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(Texture* texture, Vec2Int pos, Vec2Int size)
	: _texture(texture), _pos(pos), _size(size)
{
	_pivot = { _size.x / 2, _size.y / 2 };
}

Sprite::Sprite(Texture* texture, Vec2Int pos, Vec2Int size, Vec2Int pivot)
	: _texture(texture), _pos(pos), _size(size), _pivot(pivot)
{
}

HDC Sprite::GetDC()
{
	return _texture->GetDC();
}

int32 Sprite::GetTransparent()
{
	return _texture->GetTransparent();
}
