#pragma once
#include "ResourceBase.h"

class Texture;

class Flipbook : public ResourceBase
{
public:
	Flipbook(Texture* texture, Vec2Int size, int32 start, int32 end, int32 line, float duration, bool loop);
	Flipbook(Texture* texture, Vec2Int size, Vec2Int pivot, int32 start, int32 end, int32 line, float duration, bool loop);
	virtual ~Flipbook() {}

public:
	Texture*	GetTexture()	const { return _texture; }
	Vec2Int		GetSize()		const { return _size; }
	int32		GetStart()		const { return _start; }
	Vec2Int		GetPivot()		const { return _pivot; }
	int32		GetEnd()		const { return _end; }
	int32		GetLine()		const { return _line; }
	float		GetDuration()	const { return _duration; }
	bool		GetLoop()		const { return _loop; }

private:
	Texture*	_texture	= nullptr;
	Vec2Int		_size		= {};
	Vec2Int		_pivot = {};
	int32		_start		= 0;
	int32		_end		= 0;
	int32		_line		= 0;
	float		_duration	= 1.f;
	bool		_loop		= true;
};

