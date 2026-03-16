#pragma once
#include "Object.h"

class SyncObject : public Object
{
public:
	SyncObject() {}
	virtual ~SyncObject() {}

	virtual void OnRelease() override;

	void SetObjectId(uint64 objectId);
	uint64 GetObjectId()	const { return _objectId; }

protected:
	uint64 _objectId = {};
};

