#include "pch.h"
#include "SyncObject.h"
#include "ObjectManager.h"

void SyncObject::OnRelease()
{
	GET_SINGLE(ObjectManager)->UnregisterSyncObject(this);
}

void SyncObject::SetObjectId(uint64 objectId)
{
	_objectId = objectId;
	GET_SINGLE(ObjectManager)->RegisterSyncObject(this);
}
