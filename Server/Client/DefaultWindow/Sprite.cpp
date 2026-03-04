#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(Texture* texture, int32 x, int32 y, int32 cx, int32 cy)
	: _texture(texture), _x(x), _y(y), _cx(cx), _cy(cy)
{
	_pivotx = _cx / 2;
	_pivoty = _cy / 2;
}

Sprite::Sprite(Texture* texture, int32 x, int32 y, int32 cx, int32 cy, int32 pivotx, int32 pivoty)
	: _texture(texture), _x(x), _y(y), _cx(cx), _cy(cy), _pivotx(pivotx), _pivoty(pivoty)
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
