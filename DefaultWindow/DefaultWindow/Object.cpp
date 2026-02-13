#include "pch.h"
#include "Object.h"
#include "Component.h"

void Object::Init()
{

}

void Object::Update()
{
	for (Component* component : _components)
		component->Update();
}

void Object::Render(HDC hdc)
{
	for (Component* component : _components)
		component->Render(hdc);
}

void Object::Release()
{
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