#pragma once
#include "Object.h"
class SyncObject : public Object
{
public:
	void SetObjectId(uint64 objectId) { _objectId = objectId; }
	uint64 GetObjectId()	const { return _objectId; }

protected:
	uint64 _objectId = {};
};

