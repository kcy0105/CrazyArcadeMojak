#pragma once
class Object
{
public:
	Object() {}
	virtual ~Object() {}

	virtual void Update() {}

	static PlayerRef CreatePlayer();

public:
	Protocol::ObjectInfo info;
	GameRoomRef room;

private:
	static atomic<uint64> s_idGenerator;
};

