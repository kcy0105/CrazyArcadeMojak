#include "pch.h"
#include "Button.h"
#include "InputManager.h"
#include "TimeManager.h"

void Button::Init()
{
	__super::Init();

	SetButtonState(BS_Default);
}

void Button::Update()
{
	__super::Update();

	POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (_state == BS_Clicked)
	{
		_sumTime += deltaTime;
		if (_sumTime >= 0.2f)
		{
			_sumTime = 0.f;
			SetButtonState(BS_Default);
		}
	}

	if (IsMouseInRect())
	{
		if (GET_SINGLE(InputManager)->GetButton(KeyType::LeftMouse))
		{
			SetButtonState(BS_Pressed);
			// OnPressed
		}
		else
		{
			if (_state == BS_Pressed)
			{
				SetButtonState(BS_Clicked);
				// OnClicked
				if (_onClick)
					_onClick();
			}
		}
	}
	else
	{
		SetButtonState(BS_Default);
	}
}

void Button::Render(HDC hdc)
{
	__super::Render(hdc);

	//if (_currentSprite)
	//{
	//	::TransparentBlt(hdc,
	//		(int32)_pos.x - _size.x / 2,
	//		(int32)_pos.y - _size.y / 2,
	//		_size.x,
	//		_size.y,
	//		_currentSprite->GetDC(),
	//		_currentSprite->GetPos().x,
	//		_currentSprite->GetPos().y,
	//		_currentSprite->GetSize().x,
	//		_currentSprite->GetSize().y,
	//		_currentSprite->GetTransparent());
	//}
	//else
	{
		Utils::DrawRectInScreen(hdc, _pos, _size.x, _size.y);
	}
}

void Button::Release()
{
}

void Button::SetButtonState(ButtonState state)
{
	_state = state;

	//if (_sprites[state])
	//	SetCurrentSprite(_sprites[state]);
}
