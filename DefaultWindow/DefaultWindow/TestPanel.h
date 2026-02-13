#pragma once
#include "Panel.h"

class TestPanel : public Panel
{
public:
	TestPanel() {}
	virtual ~TestPanel() override {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void OnClickButton();
};

