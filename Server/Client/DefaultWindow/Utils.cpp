#include "pch.h"
#include "Utils.h"
#include "SceneManager.h"


void Utils::DrawText(HDC hdc, Pos pos, const wstring& str)
{
	::TextOut(hdc, static_cast<int32>(pos.x), static_cast<int32>(pos.y), str.c_str(), static_cast<int32>(str.size()));
}

void Utils::DrawRect(HDC hdc, Pos pos, int32 w, int32 h)
{
	::Rectangle(hdc, static_cast<int32>(pos.x - w / 2), static_cast<int32>(pos.y - h / 2), static_cast<int32>(pos.x + w / 2), static_cast<int32>(pos.y + h / 2));
}

void Utils::DrawCircle(HDC hdc, Pos pos, int32 radius)
{
	::Ellipse(hdc, static_cast<int32>(pos.x - radius), static_cast<int32>(pos.y - radius), static_cast<int32>(pos.x + radius), static_cast<int32>(pos.y + radius));
}

void Utils::DrawLine(HDC hdc, Pos from, Pos to)
{
	::MoveToEx(hdc, static_cast<int32>(from.x), static_cast<int32>(from.y), nullptr);
	::LineTo(hdc, static_cast<int32>(to.x), static_cast<int32>(to.y));
}

void Utils::DrawTextInWorld(HDC hdc, Pos pos, const wstring& str)
{
	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	pos.x -= (cameraPos.x - GWinSizeX / 2);
	pos.y -= (cameraPos.y - GWinSizeY / 2);

	DrawText(hdc, pos, str);
}

void Utils::DrawRectInWorld(HDC hdc, Pos pos, int32 w, int32 h)
{
	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	pos.x -= (cameraPos.x - GWinSizeX / 2);
	pos.y -= (cameraPos.y - GWinSizeY / 2);

	DrawRect(hdc, pos, w, h);
}

void Utils::DrawCircleInWorld(HDC hdc, Pos pos, int32 radius)
{
	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	pos.x -= (cameraPos.x - GWinSizeX / 2);
	pos.y -= (cameraPos.y - GWinSizeY / 2);

	DrawCircle(hdc, pos, radius);
}

void Utils::DrawLineInWorld(HDC hdc, Pos from, Pos to)
{
	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	from.x -= (cameraPos.x - GWinSizeX / 2);
	from.y -= (cameraPos.y - GWinSizeY / 2);
	to.x -= (cameraPos.x - GWinSizeX / 2);
	to.y -= (cameraPos.y - GWinSizeY / 2);

	DrawLine(hdc, from, to);
}

void Utils::DrawTextInScreen(HDC hdc, Pos pos, const wstring& str)
{
	DrawText(hdc, pos, str);
}

void Utils::DrawRectInScreen(HDC hdc, Pos pos, int32 w, int32 h)
{
	DrawRect(hdc, pos, w, h);
}

void Utils::DrawCircleInScreen(HDC hdc, Pos pos, int32 radius)
{
	DrawCircle(hdc, pos, radius);
}

void Utils::DrawLineInScreen(HDC hdc, Pos from, Pos to)
{
	DrawLine(hdc, from, to);
}

Vec2 Utils::GetDirection(float angle)
{
	return Vec2(::cos(angle * DEG2RAD), -::sin(angle * DEG2RAD));
}

