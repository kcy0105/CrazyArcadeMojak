#pragma once
#include "Component.h"

class BoxCollider;
class CircleCollider;

class Collider : public Component
{
public:
	Collider(ColliderType colliderType) : _colliderType(colliderType) {}
	virtual ~Collider() override {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	virtual bool CheckCollision(Collider* other) = 0;
	
	ColliderType GetColliderType() { return _colliderType; }
	void SetShowDebug(bool bShowDebug) { _showDebug = bShowDebug; }

public:
	static bool CheckCollisionBox2Box(BoxCollider* b1, BoxCollider* b2);
	static bool CheckCollisionCircle2Box(CircleCollider* s1, BoxCollider* b2);
	static bool CheckCollisionCircle2Circle(CircleCollider* s1, CircleCollider* s2);
protected:
	ColliderType _colliderType = {};
	bool _showDebug = true;

public:
	unordered_set<Collider*> _collisionMap;
};

