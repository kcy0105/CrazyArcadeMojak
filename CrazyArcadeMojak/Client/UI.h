#pragma once
#include "SceneManager.h"

class UI
{
public:
	UI() {}
	virtual ~UI() {}

	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void SetPos(Vec2 pos) { _pos = pos; }
	Vec2 GetPos() { return _pos; }

	void SetSize(Vec2Int size) { _size = size; }
	Vec2Int GetSize() { return _size; }

	RECT GetRect();
	bool IsMouseInRect();

protected:
	virtual void OnInit() {}
	virtual void OnUpdate() {}
	virtual void OnRender(HDC hdc) {}
	virtual void OnRelease() {}

protected:
	Vec2	_pos = { 400, 300 };
	Vec2Int _size = { 150, 150 };

public:
	template<typename T>
	static T* CreateUI()
	{
		static_assert(std::is_convertible_v<T*, UI*>);

		T* ui = new T();
		ui->Init();

		GET_SINGLE(SceneManager)->GetScene()->RegisterUI(ui);

		return ui;
	}

	template<typename T>
	static T* CreateUI(UI* parentUI)
	{
		static_assert(std::is_convertible_v<T*, UI*>);

		T* ui = new T();
		ui->Init();

		parentUI->AddChild(ui);

		return ui;
	}

	static void DestroyUI(UI* ui) { ui->Destroy(); }

protected:
	void Destroy() { _isDead = true; }

public:
	bool IsDead() { return _isDead; }

public:
	void AddChild(UI* ui);

private:
	bool _isDead = false;
	vector<UI*> _children;
};
