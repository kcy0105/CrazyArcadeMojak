#include "pch.h"
#include "Flipbook.h"

Flipbook::Flipbook(Texture* texture, Vec2Int size, int32 start, int32 end, int32 line, float duration, bool loop)
	: _texture(texture), _size(size), _start(start), _end(end), _line(line), _duration(duration), _loop(loop)
{
	_pivot = { size.x / 2, size.y / 2 };
}

Flipbook::Flipbook(Texture* texture, Vec2Int size, Vec2Int pivot, int32 start, int32 end, int32 line, float duration, bool loop)
	: _texture(texture), _size(size), _pivot(pivot), _start(start), _end(end), _line(line), _duration(duration), _loop(loop)
{
}
