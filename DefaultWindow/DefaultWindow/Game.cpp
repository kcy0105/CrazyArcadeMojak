#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "InputManager.h"

Game::Game()
{
}

Game::~Game()
{
	_CrtDumpMemoryLeaks();
	Release();
}

void Game::Init()
{
	_hdc = GetDC(g_hwnd);

	::GetClientRect(g_hwnd, &_rect);

	_hdcBack = ::CreateCompatibleDC(_hdc);
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom);
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack);
	::DeleteObject(prev);

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init();
	GET_SINGLE(SceneManager)->Init();

	GET_SINGLE(SceneManager)->ChangeScene(SceneType::GameScene);
}

void Game::Update()
{
	GET_SINGLE(TimeManager)->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(SceneManager)->Update();
}

void Game::Render()
{
	GET_SINGLE(SceneManager)->Render(_hdcBack);

	uint32 fps = GET_SINGLE(TimeManager)->GetFps();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	{
		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		::TextOut(_hdcBack, 20, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	{
		wstring str = std::format(L"FPS({0}), DT({1})", fps, deltaTime);
		::TextOut(_hdcBack, 550, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	// Double Buffering
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}

void Game::Release()
{
}
