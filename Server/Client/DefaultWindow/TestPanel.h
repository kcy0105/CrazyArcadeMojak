#pragma once
#include "Panel.h"

class TestPanel : public Panel
{
public:
	TestPanel() {}
	virtual ~TestPanel() override {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;

	void OnClickButton();
};

