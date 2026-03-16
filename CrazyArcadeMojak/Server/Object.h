#pragma once
class Object
{
public:
	Object(OBJECT_TYPE type) : _objectType(type) { _objectId = Object::s_idGenerator++; }
	virtual ~Object() {}

	virtual void Update() {}

public:
	GameRoomRef room;

public:
	void SetObjectId(uint64 objectId) { _objectId = objectId; }
	void SetPos(Pos pos) { _pos = pos; }

	uint64 GetObjectId()		const { return _objectId; }
	OBJECT_TYPE GetObjectType() const { return _objectType; }
	Pos GetPos()				const { return _pos; }
	Vec2Int GetTilePos()		const { return Utils::WorldToTile(_pos); }

protected:
	uint64 _objectId = {};
	OBJECT_TYPE _objectType = {};
	Pos _pos;

public:
	void Destroy() { _isDead = true; }

public:
	bool IsDead() const { return _isDead; }

private:
	bool _isDead = false;

public:
	static atomic<uint64> s_idGenerator;
};

