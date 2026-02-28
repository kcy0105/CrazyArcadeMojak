#pragma once
#include "Component.h"

class Sprite;

class SpriteComponent : public Component
{
public:
	SpriteComponent() {}
	virtual ~SpriteComponent() override {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }

private:
	Sprite* _sprite = nullptr;
};

