#pragma once
#include "Object.h"

class Player : public Object
{
public:
	Player() {}
	virtual ~Player() {}

	virtual void Update() override;

public:
	GameSessionRef session;
};

