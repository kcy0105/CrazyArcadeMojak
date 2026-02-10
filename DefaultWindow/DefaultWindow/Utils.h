#pragma once
#include <string>
#include "SceneManager.h"
#include "Scene.h"
using namespace std;

class Object;

class Utils
{
public:
	static void DrawText(HDC hdc, Pos pos, const wstring& str);
	static void DrawRect(HDC hdc, Pos pos, int32 w, int32 h);
	static void DrawCircle(HDC hdc, Pos pos, int32 radius);
	static void DrawLine(HDC hdc, Pos from, Pos to);
};

