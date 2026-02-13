#pragma once

class Object;

class Utils
{
private:
	static void DrawText(HDC hdc, Pos pos, const wstring& str);
	static void DrawRect(HDC hdc, Pos pos, int32 w, int32 h);
	static void DrawCircle(HDC hdc, Pos pos, int32 radius);
	static void DrawLine(HDC hdc, Pos from, Pos to);

public:
	static void DrawTextInWorld(HDC hdc, Pos pos, const wstring& str);
	static void DrawRectInWorld(HDC hdc, Pos pos, int32 w, int32 h);
	static void DrawCircleInWorld(HDC hdc, Pos pos, int32 radius);
	static void DrawLineInWorld(HDC hdc, Pos from, Pos to);

public:
	static void DrawTextInScreen(HDC hdc, Pos pos, const wstring& str);
	static void DrawRectInScreen(HDC hdc, Pos pos, int32 w, int32 h);
	static void DrawCircleInScreen(HDC hdc, Pos pos, int32 radius);
	static void DrawLineInScreen(HDC hdc, Pos from, Pos to);

public:
	static Vec2 GetDirection(float angle);
};

