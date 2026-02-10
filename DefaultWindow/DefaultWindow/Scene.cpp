#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "CollisionManager.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
}

void Scene::Update()
{
	UpdateObjects();

	GET_SINGLE(CollisionManager)->Update();

	RemoveDeadObjects();
}

void Scene::Render(HDC hdc)
{
	RenderObjects(hdc);
}

void Scene::Release()
{
	for (Object* obj : _objects)
	{
		obj->Release();
		SAFE_DELETE(obj);
	}
	_objects.clear();
}

void Scene::RegisterObject(Object* obj)
{
	_objects.push_back(obj);
}

void Scene::UnregisterObject(Object* obj)
{
	if (obj == nullptr)
		return;

	auto it = std::remove(_objects.begin(), _objects.end(), obj);
	_objects.erase(it, _objects.end());
}


void Scene::RenderObjects(HDC hdc)
{
	for (Object* obj : _objects)
	{
		obj->Render(hdc);
	}
}

void Scene::UpdateObjects()
{
	const vector<Object*> objects = _objects;
	for (Object* obj : objects)
	{
		obj->Update();
	}
}

void Scene::RemoveDeadObjects()
{
	const vector<Object*> objects = _objects;
	for (Object* obj : objects)
	{
		if (obj->IsDead())
		{
			UnregisterObject(obj);
			obj->Release();
			SAFE_DELETE(obj);
		}
	}
}
