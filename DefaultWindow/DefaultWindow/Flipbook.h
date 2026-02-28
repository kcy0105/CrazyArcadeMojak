#pragma once
#include "ResourceBase.h"

class Texture;

class Flipbook : public ResourceBase
{
public:
	Flipbook(Texture* texture, Vec2Int size, int32 start, int32 end, int32 line, float duration, bool loop);
	virtual ~Flipbook() {}

public:
	Texture*	GetTexture()	{ return _texture; }
	Vec2Int		GetSize()		{ return _size; }
	int32		GetStart()		{ return _start; }
	int32		GetEnd()		{ return _end; }
	int32		GetLine()		{ return _line; }
	float		GetDuration()	{ return _duration; }
	bool		GetLoop()		{ return _loop; }

private:
	Texture*	_texture	= nullptr;
	Vec2Int		_size		= {};
	int32		_start		= 0;
	int32		_end		= 0;
	int32		_line		= 0;
	float		_duration	= 1.f;
	bool		_loop		= true;
};

