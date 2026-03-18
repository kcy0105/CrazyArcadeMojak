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

	// 단일 Flipbook
	void SetFlipbook(Flipbook* flipbook);

	// 여러 Flipbook (체인 재생)
	void SetFlipbook(const vector<Flipbook*>& flipbooks);

private:
	Flipbook* GetCurrentFlipbook();

private:
	vector<Flipbook*> _flipbooks;
	int32 _fbIndex = 0;

	float _sumTime = 0.f;
	int32 _idx = 0;
};