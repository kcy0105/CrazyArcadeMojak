#pragma once
#include "Scene.h"

class DevScene : public Scene
{
public:
	DevScene() {}
	virtual ~DevScene() override {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRelease() override;
};

