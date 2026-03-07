#include "pch.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"
#include "WaterBomb.h"
#include "DevScene.h"
#include "Tilemap.h"

void MyPlayer::OnUpdate()
{
	_moveDirtyFlag = false;

	__super::OnUpdate();

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetScene());
		if (scene->GetMapObjectAt(GetPos()) == nullptr)
		{
			auto bomb = Object::CreateObject<WaterBomb>();
			bomb->SetPos(Utils::TileToWorld(GetTilePos()));
			bomb->SetOwner(this);
			bomb->SetOwnerCanPass(true);

			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_WaterBomb(GetTilePos().x, GetTilePos().y);
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}		
	}

	if (_moveDirtyFlag)
		SyncToServer();
}

void MyPlayer::OnUpdateIdle()
{
	__super::OnUpdateIdle();

	if (GET_SINGLE(InputManager)->GetButton(KeyType::Left))
	{
		SetDir(DIR_LEFT);
		SetState(PLAYER_STATE_MOVE);
		_moveDirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::Right))
	{
		SetDir(DIR_RIGHT);
		SetState(PLAYER_STATE_MOVE);
		_moveDirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::Up))
	{
		SetDir(DIR_UP);
		SetState(PLAYER_STATE_MOVE);
		_moveDirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::Down))
	{
		SetDir(DIR_DOWN);
		SetState(PLAYER_STATE_MOVE);
		_moveDirtyFlag = true;
	}
}

void MyPlayer::OnUpdateMove()
{
	__super::OnUpdateMove(); 

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Left))
	{
		SetDir(DIR_LEFT);
		SetState(PLAYER_STATE_MOVE);
		_moveDirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Right))
	{
		SetDir(DIR_RIGHT);
		SetState(PLAYER_STATE_MOVE);
		_moveDirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Up))
	{
		SetDir(DIR_UP);
		SetState(PLAYER_STATE_MOVE);
		_moveDirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Down))
	{
		SetDir(DIR_DOWN);
		SetState(PLAYER_STATE_MOVE);
		_moveDirtyFlag = true;
	}

	if (!GET_SINGLE(InputManager)->GetButton(KeyType::Left)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Right)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Up)
		&& !GET_SINGLE(InputManager)->GetButton(KeyType::Down))
	{
		SetState(PLAYER_STATE_IDLE); 

		_moveDirtyFlag = true;
	}
}

void MyPlayer::SyncToServer()
{
	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move(GetObjectId(), GetState(), GetDir(), GetPos().x, GetPos().y);
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}
