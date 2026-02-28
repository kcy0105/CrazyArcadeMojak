#include "pch.h"
#include "Object.h"
#include "Component.h"

void Object::Init()
{
	OnInit();
}

void Object::Update()
{
	OnUpdate();
	for (Component* component : _components)
		component->Update();
}

void Object::LateUpdate()
{
	OnLateUpdate();
	for (Component* component : _components)
		component->LateUpdate();
}

void Object::Render(HDC hdc)
{
	OnRender(hdc);
	for (Component* component : _components)
		component->Render(hdc);
}

void Object::Release()
{
	OnRelease();
	for (Component* component : _components)
	{
		component->Release();
		SAFE_DELETE(component);
	}

	_components.clear();
}

void Object::RemoveComponent(Component* component)
{
	auto it = std::remove(_components.begin(), _components.end(), component);
	_components.erase(it);

	component->Release();
	SAFE_DELETE(component);
}