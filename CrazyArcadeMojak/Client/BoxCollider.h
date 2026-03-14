#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
public:
	BoxCollider() : Collider(ColliderType::Box) {}
	virtual ~BoxCollider() override {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void DebugRender(HDC hdc) override;

	virtual bool CheckCollision(Collider* other) override;

	Vec2 GetSize() const { return _size; }
	void SetSize(Vec2 size) { _size = size; }

	RECT GetRect();

private:
	Vec2 _size = {};
};

