#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
public:
	BoxCollider() : Collider(ColliderType::Box) {}
	virtual ~BoxCollider() override {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	virtual bool CheckCollision(Collider* other) override;

	Vec2 GetSize() { return _size; }
	void SetSize(Vec2 size) { _size = size; }

private:
	Vec2 _size = {};
};

