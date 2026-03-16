#pragma once
#include "Object.h"

class FlipbookRenderer;

class FlowEffect : public Object
{
public:
	FlowEffect() {}
	virtual ~FlowEffect() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;

public:
	void SetInfo(DIR dir, bool isEnd);

private:
	FlipbookRenderer* _fb = {};
	float _timer = 0;
};

