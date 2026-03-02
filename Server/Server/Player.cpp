#include "pch.h"
#include "Player.h"
#include "TimeManager.h"

void Player::Update()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	switch (info.state())
	{
	case Protocol::PLAYER_STATE_IDLE:
		break;
	case Protocol::PLAYER_STATE_MOVE:
	{
		float posx = info.posx();
		float posy = info.posy();

		float moveSpeed = 200;

		switch (info.dir())
		{
		case Protocol::DIR_LEFT:
			posx -= moveSpeed * deltaTime;
			break;
		case Protocol::DIR_RIGHT:
			posx += moveSpeed * deltaTime;
			break;
		case Protocol::DIR_UP:
			posy -= moveSpeed * deltaTime;
			break;
		case Protocol::DIR_DOWN:
			posy += moveSpeed * deltaTime;
			break;
		}

		info.set_posx(posx);
		info.set_posy(posy);
	}

		
		break;
	}
}
