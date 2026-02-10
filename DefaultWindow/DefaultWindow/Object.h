#pragma once

class Component;
class Collider;

class Object
{
public:
	Object();
	virtual ~Object();

public:
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

public:
	Pos GetPos() const { return _pos; }
	void SetPos(Pos pos) { _pos = pos; }
	ObjectType GetObjectType() const { return _objectType; }
	void SetObjectType(ObjectType objectType) { _objectType = objectType; }

protected:
	Pos _pos = {};
	ObjectType _objectType = ObjectType::None;

public:
	template<typename T>
	T* AddComponent()
	{
		static_assert(std::is_convertible_v<T*, Component*>);

		T* component = new T();
		component->SetOwner(this);
		_components.push_back(component);
		component->Init();

		return component;
	}
	void RemoveComponent(Component* component);

protected:
	vector<Component*> _components;

public:
	virtual void OnColliderBeginOverlap(Collider* collider, Collider* other);
	virtual void OnColliderEndOverlap(Collider* collider, Collider* other);

public:
	template<typename T>
	static T* CreateObject()
	{
		static_assert(std::is_convertible_v<T*, Object*>);

		T* obj = new T();
		obj->Init();

		GET_SINGLE(SceneManager)->GetScene()->RegisterObject(obj);

		return obj;
	}
	static void DestroyObject(Object* obj);

protected:
	void Destroy() { _isDead = true; }

public:
	bool IsDead() const { return _isDead; }

private:
	bool _isDead = false;
};

