#pragma once
#include "BlockingObject.h"

class Player;

class WaterBomb : public BlockingObject
{
public:
	WaterBomb() {}
	virtual ~WaterBomb() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnRelease() override;

	virtual void OnColliderEndOverlap(Collider* collider, Collider* other) override;

public:
	void SetOwner(Player* owner)			{ _owner = owner; }
	void SetOwnerCanPass(bool ownerCanPass) { _ownerCanPass = ownerCanPass; }
	
	Player* GetOwner()				const	{ return _owner; }
	bool	GetOwnerCanPass()		const	{ return _ownerCanPass; }

private:
	Player* _owner = nullptr;

	bool _ownerCanPass = true;
};

