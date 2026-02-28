#include "pch.h"
#include "Image.h"
#include "Sprite.h"

void Image::OnInit()
{
}

void Image::OnUpdate()
{
}

void Image::OnRender(HDC hdc)
{
	if (_sprite)
	{
		::TransparentBlt(hdc,
			(int32)_pos.x - _size.x / 2,
			(int32)_pos.y - _size.y / 2,
			_size.x,
			_size.y,
			_sprite->GetDC(),
			_sprite->GetPos().x,
			_sprite->GetPos().y,
			_sprite->GetSize().x,
			_sprite->GetSize().y,
			_sprite->GetTransparent());
	}
}

void Image::OnRelease()
{
}
