#pragma once

class Scene;

class SceneManager
{
	DECLARE_SINGLE(SceneManager)

public:
	void Init();
	void Update();
	void Render(HDC hdc);

	void Clear();

public:
	void ChangeScene(SceneType sceneType);
	Scene* GetScene() { return _scene; }

private:
	class Scene* _scene = nullptr;
	SceneType _sceneType = SceneType::None;
};

