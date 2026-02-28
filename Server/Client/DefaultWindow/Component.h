#pragma once
class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Init() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void Render(HDC hdc) {}
	virtual void Release() {}

	void SetOwner(Object* owner) { _owner = owner; }
	Object* GetOwner() { return _owner; }

protected:
	Object* _owner = {};
};

