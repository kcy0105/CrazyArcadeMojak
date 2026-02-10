#pragma once
class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Render(HDC hdc) abstract;
	virtual void Release() abstract;

	void SetOwner(Object* owner) { _owner = owner; }
	Object* GetOwner() { return _owner; }

protected:
	Object* _owner = {};
};

