#pragma once
#include "Object.h"

class Player : public Object
{
public:
	Player();
	virtual ~Player();

public:
	GameSessionRef session;
};

