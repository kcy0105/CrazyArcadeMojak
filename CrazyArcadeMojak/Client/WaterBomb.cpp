#include "pch.h"
#include "WaterBomb.h"
#include "FlipbookRenderer.h"
#include "ResourceManager.h"
#include "Player.h"
#include "FlowEffect.h"

void WaterBomb::OnInit()
{
	__super::OnInit();

	FlipbookRenderer* fb = AddComponent<FlipbookRenderer>();
	fb->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_WaterBomb_Idle"));
}

void WaterBomb::OnUpdate()
{
}

void WaterBomb::OnRender(HDC hdc)
{

}

void WaterBomb::Explode(int32 up, int32 down, int32 left, int32 right)
{
	{
		FlowEffect* flow = Object::CreateObject<FlowEffect>();
		flow->SetPos(GetPos());
	}

	vector<Vec2> dirs = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	vector<int32> counts = { up, down, left, right };
	
	for (int i = 0; i < 4; i++)
	{
		int32 count = counts[i];
		Vec2 dir = dirs[i];
		for (int j = 1; j<=count; j++)
		{
			Pos pos = GetPos() + TILE_SIZE * j * dir;
			if (j < count)
			{
				FlowEffect* flow = Object::CreateObject<FlowEffect>();
				flow->SetPos(pos);
				flow->SetInfo((DIR)i, false);
			}
			else
			{
				FlowEffect* flow = Object::CreateObject<FlowEffect>();
				flow->SetPos(pos);
				flow->SetInfo((DIR)i, true);
			}
		}
	}

	Destroy();
}



