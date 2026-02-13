#pragma once
class Object;
class UI;

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

	void RegisterUI(UI* ui);
	void UnregisterUI(UI* ui);

private:
	void UpdateObjects();
	void RenderObjects(HDC hdc);
	void RemoveDeadObjects();
	void ReleaseObjects();

	void UpdateUIs();
	void RenderUIs(HDC hdc);
	void RemoveDeadUIs();
	void ReleaseUIs();

private:
	vector<Object*> _objects;
	vector<UI*> _uis;
};

