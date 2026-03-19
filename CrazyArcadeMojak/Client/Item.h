#pragma once
#include "MapObject.h"

class SpriteRenderer;

class Item : public MapObject
{
public:
	virtual void OnInit() override;
	virtual void OnDebugRender(HDC hdc) override;

protected:
	SpriteRenderer* _spr;

public:
	virtual bool BlocksPlayer(const Player* player) const override { return false; }
};

