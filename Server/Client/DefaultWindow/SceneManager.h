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

public:
	Vec2 GetCameraPos() { return _cameraPos; }
	void SetCameraPos(Vec2 pos) { _cameraPos = pos; }

private:
	Vec2 _cameraPos = { GWinSizeX / 2, GWinSizeY / 2 };
};

