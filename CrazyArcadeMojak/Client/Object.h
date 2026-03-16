#pragma once
#include "SceneManager.h"
#include "Scene.h"

class Component;
class Collider;

class Object
{
public:
	Object() {};
	virtual ~Object() {};

public:
	void Init();
	void Update();
	void LateUpdate();
	void Render(HDC hdc);
	void DebugRender(HDC hdc);
	void Release();

protected:
	virtual void OnInit() {}
	virtual void OnUpdate() {}
	virtual void OnLateUpdate() {}
	virtual void OnRender(HDC hdc) {}
	virtual void OnDebugRender(HDC hdc) {}
	virtual void OnRelease() {}
	
public: // 충돌 처리 필요 시 override
	virtual void OnColliderBeginOverlap(Collider* collider, Collider* other) {}
	virtual void OnColliderEndOverlap(Collider* collider, Collider* other) {}
	virtual void OnColliderStayOverlap(Collider* collider, Collider* other) {}

public:
	Pos GetPos() { return _pos; }
	void SetPos(Pos pos) { _pos = pos; }
	wstring GetTag() { return _tag; }
	void SetTag(wstring tag) { _tag = tag; }

protected:
	Pos _pos = {};
	wstring _tag = {};

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
	template<typename T>
	static T* CreateObject()
	{
		static_assert(std::is_convertible_v<T*, Object*>);

		T* obj = new T();
		obj->Init();

		GET_SINGLE(SceneManager)->GetScene()->RegisterObject(obj);

		return obj;
	}
	static void DestroyObject(Object* obj) { obj->Destroy(); }

protected:
	void Destroy() { _isDead = true; }

public:
	bool IsDead() const { return _isDead; }

private:
	bool _isDead = false;
};

