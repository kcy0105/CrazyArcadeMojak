#pragma once
class Game
{
public:
	Game();
	~Game();

public:
	void Init();
	void Update();
	void Render();
	void Release();

private:
	HDC _hdc = {};

private:
	// Double Buffering
	RECT _rect = {};
	HDC _hdcBack = {};
	HBITMAP _bmpBack = {};
};

