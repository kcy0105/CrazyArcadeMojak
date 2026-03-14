#pragma once

class Collider;

class CollisionManager
{
	DECLARE_SINGLE(CollisionManager)

public:
	void Init();
	void Update();

	void RegisterCollider(Collider* collider);
	void UnregisterCollider(Collider* collider);

private:
	vector<Collider*> _colliders;
};

