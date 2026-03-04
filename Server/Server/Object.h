#pragma once
class Object
{
public:
	Object() {}
	virtual ~Object() {}

	virtual void Update() {}

	static PlayerRef CreatePlayer();




public:
	//Protocol::ObjectInfo info;
	GameRoomRef room;

public:
	void SetObjectId(uint64 objectId) { _objectId = objectId; }
	uint64 GetObjectId() { return _objectId; }
	void SetObjectType(OBJECT_TYPE objectType) { _objectType = objectType; }
	OBJECT_TYPE GetObjectType() { return _objectType; }
	void SetPos(Pos pos) { _pos = pos; }
	Pos GetPos() { return _pos; }

private:
	uint64 _objectId = {};
	OBJECT_TYPE _objectType = {};
	Pos _pos;

private:
	static atomic<uint64> s_idGenerator;
};

