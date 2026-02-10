#pragma once
class Object;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init();
	virtual void Update();
	void Render(HDC hdc);
	virtual void Release();

	void RegisterObject(Object* obj);
	void UnregisterObject(Object* obj);

private:
	void RenderObjects(HDC hdc);
	void UpdateObjects();
	void RemoveDeadObjects();

private:
	vector<Object*> _objects;
};

