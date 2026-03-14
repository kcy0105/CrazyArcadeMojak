#pragma once
#include "Component.h"

class Flipbook;

class FlipbookRenderer : public Component
{
public:
	FlipbookRenderer() {}
	virtual ~FlipbookRenderer() override {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void SetFlipbook(Flipbook* flipbook) { _flipbook = flipbook; }

private:
	Flipbook* _flipbook = nullptr;
	float _sumTime = 0.f;
	int32 _idx = 0;
};

