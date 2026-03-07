#include "pch.h"
#include "BlockingObject.h"
#include "BoxCollider.h"

void BlockingObject::OnInit()
{
	BoxCollider* box = AddComponent<BoxCollider>();
	box->SetSize({ 40, 40 });
}

void BlockingObject::OnUpdate()
{
}

void BlockingObject::OnRender(HDC hdc)
{
}

void BlockingObject::OnRelease()
{
}
