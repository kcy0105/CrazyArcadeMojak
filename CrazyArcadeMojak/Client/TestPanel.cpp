#include "pch.h"
#include "TestPanel.h"
#include "Button.h"

void TestPanel::OnInit()
{
	{
		Button* button = UI::CreateUI<Button>(this);
		button->SetPos({ 400, 500 });
		button->SetSize({ 150, 50 });
		button->AddOnClickDelegate(this, &TestPanel::OnClickButton);
	}
}

void TestPanel::OnUpdate()
{
}

void TestPanel::OnRender(HDC hdc)
{
}

void TestPanel::OnRelease()
{
}

void TestPanel::OnClickButton()
{
	int a = 3;
}
