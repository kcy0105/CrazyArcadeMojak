#pragma once
#include "Object.h"

class BoxCollider;

class Wall :public Object
{
public:
	Wall();
	virtual ~Wall() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	Vec2 GetSize() { return _size; }
	void SetSize(Vec2 size);

private:
	Vec2 _size = {};
	BoxCollider* _boxCollider = {};
};