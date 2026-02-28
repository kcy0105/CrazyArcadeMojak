#include "pch.h"
#include "Flipbook.h"

Flipbook::Flipbook(Texture* texture, Vec2Int size, int32 start, int32 end, int32 line, float duration, bool loop)
	: _texture(texture), _size(size), _start(start), _end(end), _line(line), _duration(duration), _loop(loop)
{
}
