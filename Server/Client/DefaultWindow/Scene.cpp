#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "CollisionManager.h"
#include "UI.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	OnInit();
}

void Scene::Update()
{
	OnUpdate();

	UpdateObjects();
	LateUpdateObjects();

	GET_SINGLE(CollisionManager)->Update();

	UpdateUIs();

	RemoveDeadObjects();
}

void Scene::Render(HDC hdc)
{
	OnRender(hdc);

	RenderObjects(hdc);
	RenderUIs(hdc);
}

void Scene::Release()
{
	OnRelease();

	ReleaseObjects();
	ReleaseUIs();
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

void Scene::RegisterUI(UI* ui)
{
	_uis.push_back(ui);
}

void Scene::UnregisterUI(UI* ui)
{
	if (ui == nullptr)
		return;

	auto it = std::remove(_uis.begin(), _uis.end(), ui);
	_uis.erase(it, _uis.end());
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

void Scene::LateUpdateObjects()
{
	const vector<Object*> objects = _objects;
	for (Object* obj : objects)
	{
		obj->LateUpdate();
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

void Scene::ReleaseObjects()
{
	for (Object* obj : _objects)
	{
		obj->Release();
		SAFE_DELETE(obj);
	}
	_objects.clear();
}

void Scene::UpdateUIs()
{
	const vector<UI*> uis = _uis;
	for (UI* ui : uis)
	{
		ui->Update();
	}
}

void Scene::RenderUIs(HDC hdc)
{
	for (UI* ui : _uis)
	{
		ui->Render(hdc);
	}
}

void Scene::RemoveDeadUIs()
{
	const vector<UI*> uis = _uis;
	for (UI* ui : uis)
	{
		if (ui->IsDead())
		{
			UnregisterUI(ui);
			ui->Release();
			SAFE_DELETE(ui);
		}
	}
}

void Scene::ReleaseUIs()
{
	for (UI* ui : _uis)
	{
		ui->Release();
		SAFE_DELETE(ui);
	}
	_uis.clear();
}

