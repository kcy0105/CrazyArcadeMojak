#pragma once
#include "UI.h"
class Sprite;

class Image : public UI
{
public:
	Image() {}
	virtual ~Image() override {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }

private:
	Sprite* _sprite = nullptr;
};

