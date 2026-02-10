#pragma once

class CComponent;
class CCollider;

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	Pos GetPos() const { return m_pos; }
	void SetPos(Pos pos) { m_pos = pos; }
	ObjectType GetObjectType() const { return m_objectType; }
	void SetObjectType(ObjectType objectType) { m_objectType = objectType; }

protected:
	Pos m_pos = {};
	ObjectType m_objectType = ObjectType::None;

public:
	template<typename T>
	T* AddComponent()
	{
		static_assert(std::is_convertible_v<T*, CComponent*>);

		T* pComponent = new T();
		pComponent->SetOwner(this);
		m_vecComponents.push_back(pComponent);
		pComponent->Initialize();

		return pComponent;
	}
	void RemoveComponent(CComponent* component);

protected:
	vector<CComponent*> m_vecComponents;

public:
	virtual void OnColliderBeginOverlap(CCollider* collider, CCollider* other);
	virtual void OnColliderEndOverlap(CCollider* collider, CCollider* other);

public:
	template<typename T>
	static T* CreateObject()
	{
		static_assert(std::is_convertible_v<T*, CObj*>);

		T* pObj = new T();
		pObj->Initialize();

		GET_SINGLE(CSceneManager)->GetScene()->RegisterObject(pObj);

		return pObj;
	}
	static void DestroyObject(CObj* pObj);

protected:
	void Destroy() { bIsDead = true; }

public:
	bool IsDead() { return bIsDead; }

private:
	bool bIsDead = false;
};

