#pragma once
class Object
{
public:
	Object() {}
	virtual ~Object() {}

	virtual void Update() {}

public:
	GameRoomRef room;

public:
	void SetObjectId(uint64 objectId) { _objectId = objectId; }
	uint64 GetObjectId() { return _objectId; }
	void SetObjectType(OBJECT_TYPE objectType) { _objectType = objectType; }
	OBJECT_TYPE GetObjectType() { return _objectType; }
	void SetPos(Pos pos) { _pos = pos; }
	Pos GetPos() { return _pos; }

protected:
	uint64 _objectId = {};
	OBJECT_TYPE _objectType = {};
	Pos _pos;

public:
	static atomic<uint64> s_idGenerator;
};

