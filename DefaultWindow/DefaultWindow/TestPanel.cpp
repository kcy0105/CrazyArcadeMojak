#include "pch.h"
#include "TestPanel.h"
#include "Button.h"

void TestPanel::Init()
{
	__super::Init();

	{
		Button* button = UI::CreateUI<Button>(this);
		button->SetPos({ 400, 500 });
		button->SetSize({ 150, 50 });
		button->AddOnClickDelegate(this, &TestPanel::OnClickButton);
	}
}

void TestPanel::Update()
{
	__super::Update();
}

void TestPanel::Render(HDC hdc)
{
	__super::Render(hdc);
}

void TestPanel::Release()
{
	__super::Release();
}

void TestPanel::OnClickButton()
{
	int a = 3;
}
