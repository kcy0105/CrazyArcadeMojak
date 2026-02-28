#pragma once
#include "Collider.h"
class CircleCollider : public Collider
{
public:
	CircleCollider() : Collider(ColliderType::Circle) {}
	virtual ~CircleCollider() override {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	virtual bool CheckCollision(Collider* other) override;

	float GetRadius() { return _radius; }
	void SetRadius(float radius) { _radius = radius; }


private:
	float _radius = 0.f;
};

