#include "pch.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"
#include "WaterBomb.h"
#include "DevScene.h"
#include "Tilemap.h"
#include "ObjectManager.h"

void MyPlayer::OnUpdate()
{
	_moveDirtyFlag = false;

	__super::OnUpdate();

	if (_moveDirtyFlag)
		SyncToServer();
}

void MyPlayer::OnUpdateNormal()
{
	HandleBombInput();

	switch (_moveState)
	{
	case MOVE_STATE_IDLE:
		HandleMoveInput_Idle();
		break;
	case MOVE_STATE_MOVE:
		HandleMoveInput_Move();
		Move();
		CheckOverlapBombs();
		break;
	}
}

void MyPlayer::OnUpdateTrapped()
{
	switch (_moveState)
	{
	case MOVE_STATE_IDLE:
		HandleMoveInput_Idle();
		break;
	case MOVE_STATE_MOVE:
		HandleMoveInput_Move();
		Move();
		break;
	}
}

void MyPlayer::OnUpdateDead()
{
}

void MyPlayer::HandleMoveInput_Idle()
{
	bool up = GET_SINGLE(InputManager)->GetButton(KeyType::Up);
	bool down = GET_SINGLE(InputManager)->GetButton(KeyType::Down);
	bool left = GET_SINGLE(InputManager)->GetButton(KeyType::Left);
	bool right = GET_SINGLE(InputManager)->GetButton(KeyType::Right);

	if (up)
		SetDir(DIR_UP);
	else if (down)
		SetDir(DIR_DOWN);
	else if (left)
		SetDir(DIR_LEFT);
	else if (right)
		SetDir(DIR_RIGHT);

	if (up || down || left || right)
	{
		SetMoveState(MOVE_STATE_MOVE);
		_moveDirtyFlag = true;
	}
}

void MyPlayer::HandleMoveInput_Move()
{
	bool up = GET_SINGLE(InputManager)->GetButtonDown(KeyType::Up);
	bool down = GET_SINGLE(InputManager)->GetButtonDown(KeyType::Down);
	bool left = GET_SINGLE(InputManager)->GetButtonDown(KeyType::Left);
	bool right = GET_SINGLE(InputManager)->GetButtonDown(KeyType::Right);

	if (up)
		SetDir(DIR_UP);
	else if (down)
		SetDir(DIR_DOWN);
	else if (left)
		SetDir(DIR_LEFT);
	else if (right)
		SetDir(DIR_RIGHT);

	if (up || down || left || right)
	{
		_moveDirtyFlag = true;
	}

	bool nothingPressed = !GET_SINGLE(InputManager)->GetButton(KeyType::Up) && !GET_SINGLE(InputManager)->GetButton(KeyType::Down) && !GET_SINGLE(InputManager)->GetButton(KeyType::Left) && !GET_SINGLE(InputManager)->GetButton(KeyType::Right);
	if (nothingPressed)
	{
		SetMoveState(MOVE_STATE_IDLE);
		_moveDirtyFlag = true;
	}
}

void MyPlayer::HandleBombInput()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		if (SpawnWaterBomb())
		{
			Protocol::C_WaterBomb pkt;
			pkt.set_ownerid(GetObjectId());
			pkt.set_tileposx(GetTilePos().x);
			pkt.set_tileposy(GetTilePos().y);

			GET_SINGLE(NetworkManager)->SendPacket(pkt);
		}
	}
}

void MyPlayer::SetMainState(PLAYER_STATE mainState)
{
	__super::SetMainState(mainState);

	switch (mainState)
	{
	case PLAYER_STATE_NORMAL:
		_moveSpeed = MOVE_SPEED_NORMAL;
		break;
	case PLAYER_STATE_TRAPPED:
		_moveSpeed = MOVE_SPEED_TRAPPED;
		break;
	}
}

void MyPlayer::Move()
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

	static float moveSyncTimer = 0.f;
	moveSyncTimer += deltaTime;

	const float MOVE_SYNC_INTERVAL = 0.05f; // 20Hz

	if (moveSyncTimer >= MOVE_SYNC_INTERVAL)
	{
		_moveDirtyFlag = true;
		moveSyncTimer = 0.f;
	}
}

void MyPlayer::CheckOverlapBombs()
{
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
}

void MyPlayer::TryMove(Pos nextPos)
{
	Pos pos = nextPos;
	float half = PLAYER_SIZE / 2;

	RECT playerRect =
	{
		(LONG)(nextPos.x - half),
		(LONG)(nextPos.y - half),
		(LONG)(nextPos.x + half),
		(LONG)(nextPos.y + half)
	};

	int32 minTileX = playerRect.left / TILE_SIZE;
	int32 maxTileX = (playerRect.right - 1) / TILE_SIZE;
	int32 minTileY = playerRect.top / TILE_SIZE;
	int32 maxTileY = (playerRect.bottom - 1) / TILE_SIZE;

	bool found = false;

	for (int32 y = minTileY; y <= maxTileY; y++)
	{
		for (int32 x = minTileX; x <= maxTileX; x++)
		{
			MapObject* obj = GET_SINGLE(ObjectManager)->GetMapObjectAt({ x, y });

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
				if (GetPos().x == nextPos.x) // y 축 이동
				{
					if (nextPos.y > GetPos().y)
						pos.y = tileRect.top - half;
					else
						pos.y = tileRect.bottom + half;
				}
				else // x 축 이동
				{
					if (nextPos.x > GetPos().x)
						pos.x = tileRect.left - half;
					else
						pos.x = tileRect.right + half;
				}
				found = true;
				break;
			}
		}
		if (found) break;
	}

	SetPos(pos);
}

bool MyPlayer::SpawnWaterBomb()
{
	if (GET_SINGLE(ObjectManager)->GetMapObjectAt(GetTilePos()) == nullptr)
	{
		auto bomb = static_cast<WaterBomb*>(GET_SINGLE(ObjectManager)->SpawnMapObject(MAP_OBJECT_TYPE_WATER_BOMB, GetTilePos()));
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
	Protocol::C_Move pkt;
	pkt.set_objectid(GetObjectId());
	pkt.set_movestate(GetMoveState());
	pkt.set_dir(GetDir());
	pkt.set_posx(GetPos().x);
	pkt.set_posy(GetPos().y);

	GET_SINGLE(NetworkManager)->SendPacket(pkt);
}
