#include "pch.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "Object.h"

void CollisionManager::Init()
{
}

void CollisionManager::Update()
{
	for (int32 i = 0; i < _colliders.size(); i++)
	{
		for (int32 j = i + 1; j < _colliders.size(); j++)
		{
			Collider* src = _colliders[i];
			Collider* dest = _colliders[j];

			if (src->CheckCollision(dest))
			{
				if (src->_collisionMap.contains(dest) == false)
				{
					src->GetOwner()->OnColliderBeginOverlap(src, dest);
					dest->GetOwner()->OnColliderBeginOverlap(dest, src);
					src->_collisionMap.insert(dest);
					dest->_collisionMap.insert(src);
				}
			}
			else
			{
				if (src->_collisionMap.contains(dest))
				{
					src->GetOwner()->OnColliderEndOverlap(src, dest);
					dest->GetOwner()->OnColliderEndOverlap(dest, src);
					src->_collisionMap.erase(dest);
					dest->_collisionMap.erase(src);
				}
			}
		}
	}
}

void CollisionManager::RegisterCollider(Collider* collider)
{
	_colliders.push_back(collider);
}

void CollisionManager::UnregisterCollider(Collider* collider)
{
	auto it = std::remove(_colliders.begin(), _colliders.end(), collider);
	_colliders.erase(it, _colliders.end());
}
