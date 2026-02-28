#pragma once
class Object;
class UI;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void RegisterObject(Object* obj);
	void UnregisterObject(Object* obj);

	void RegisterUI(UI* ui);
	void UnregisterUI(UI* ui);

protected:
	virtual void OnInit() {}
	virtual void OnUpdate() {}
	virtual void OnRender(HDC hdc) {}
	virtual void OnRelease() {}

private:
	void UpdateObjects();
	void LateUpdateObjects();
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

