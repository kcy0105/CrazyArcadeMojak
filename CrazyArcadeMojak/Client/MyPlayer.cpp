#include "pch.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"
#include "WaterBomb.h"
#include "DevScene.h"
#include "Tilemap.h"
#include "MapManager.h"

void MyPlayer::OnUpdate()
{
	_moveDirtyFlag = false;

	__super::OnUpdate();

	if (_moveDirtyFlag)
		SyncToServer();
}

void MyPlayer::OnUpdateIdle()
{
	HandleMoveInput_Idle();
	HandleBombInput();
}

void MyPlayer::OnUpdateMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Pos pos = GetPos();

	switch (_dir)
	{
	case DIR_LEFT:
		pos.x -= _moveSpeed * deltaTime;
		break;
	case DIR_RIGHT:
		pos.x += _moveSpeed * deltaTime;
		break;
	case DIR_UP:
		pos.y -= _moveSpeed * deltaTime;
		break;
	case DIR_DOWN:
		pos.y += _moveSpeed * deltaTime;
		break;
	}

	TryMove(pos);

	for (auto it = _overlapBombs.begin(); it != _overlapBombs.end();)
	{
		WaterBomb* bomb = *it;

		RECT r1 = GetRect();
		RECT r2 = bomb->GetRect();
		RECT r = {};

		if (::IntersectRect(&r, &r1, &r2) == false)
		{
			bomb->SetPassable(false);
			it = _overlapBombs.erase(it);
		}
		else
		{
			++it;
		}
	}

	static float moveSyncTimer = 0.f;
	moveSyncTimer += deltaTime;

	const float MOVE_SYNC_INTERVAL = 0.05f; // 20Hz

	if (moveSyncTimer >= MOVE_SYNC_INTERVAL)
	{
		_moveDirtyFlag = true;
		moveSyncTimer = 0.f;
	}

	HandleMoveInput_Move();
	HandleBombInput();
}

void MyPlayer::HandleMoveInput_Idle()
{
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

void MyPlayer::HandleMoveInput_Move()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Left))
	{
		SetDir(DIR_LEFT);
		_moveDirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Right))
	{
		SetDir(DIR_RIGHT);
		_moveDirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Up))
	{
		SetDir(DIR_UP);
		_moveDirtyFlag = true;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Down))
	{
		SetDir(DIR_DOWN);
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

void MyPlayer::HandleBombInput()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		if (SpawnWaterBomb())
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_WaterBomb(GetObjectId(), GetTilePos().x, GetTilePos().y);
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
	}
}

void MyPlayer::TryMove(Pos nextPos)
{
	Pos pos = GetPos();
	float half = PLAYER_SIZE / 2;

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
			pos.x = nextPos.x;
		else
			pos.y = nextPos.y;

		RECT playerRect =
		{
			(LONG)(pos.x - half),
			(LONG)(pos.y - half),
			(LONG)(pos.x + half),
			(LONG)(pos.y + half)
		};

		int32 minTileX = playerRect.left / TILE_SIZE;
		int32 maxTileX = (playerRect.right - 1) / TILE_SIZE;
		int32 minTileY = playerRect.top / TILE_SIZE;
		int32 maxTileY = (playerRect.bottom - 1) / TILE_SIZE;

		for (int32 y = minTileY; y <= maxTileY; y++)
		{
			for (int32 x = minTileX; x <= maxTileX; x++)
			{
				MapObject* obj = GET_SINGLE(MapManager)->GetMapObjectAt({ x, y });

				if (obj == nullptr)
					continue;

				if (!obj->BlocksPlayer(this))
					continue;

				RECT tileRect =
				{
					x * TILE_SIZE,
					y * TILE_SIZE,
					x * TILE_SIZE + TILE_SIZE,
					y * TILE_SIZE + TILE_SIZE
				};

				RECT intersect;
				if (::IntersectRect(&intersect, &playerRect, &tileRect))
				{
					if (i == 0)
					{
						if (nextPos.x > GetPos().x)
							pos.x = tileRect.left - half;
						else
							pos.x = tileRect.right + half;
					}
					else
					{
						if (nextPos.y > GetPos().y)
							pos.y = tileRect.top - half;
						else
							pos.y = tileRect.bottom + half;
					}
				}
			}
		}
	}

	SetPos(pos);
}

bool MyPlayer::SpawnWaterBomb()
{
	if (GET_SINGLE(MapManager)->GetMapObjectAt(GetTilePos()) == nullptr)
	{
		auto bomb = static_cast<WaterBomb*>(GET_SINGLE(MapManager)->SpawnMapObject(MAP_OBJECT_TYPE_WATER_BOMB, GetTilePos()));
		bomb->SetOwner(this);
		bomb->SetPassable(true);
		_overlapBombs.push_back(bomb);

		pendingBombs.push(bomb);

		return true;
	}
	return false;
}

void MyPlayer::SyncToServer()
{
	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move(GetObjectId(), GetState(), GetDir(), GetPos().x, GetPos().y);
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}
