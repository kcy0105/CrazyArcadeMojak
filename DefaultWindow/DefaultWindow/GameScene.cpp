#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Wall.h"
#include "Monster.h"
#include "Camera.h"
#include "UI.h"
#include "TestPanel.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	__super::Init();

	{
		Wall* wall = Object::CreateObject<Wall>();
		wall->SetPos({ 300, 50 });
		wall->SetSize({ 800, 100 });
	}

	{
		Wall* wall = Object::CreateObject<Wall>();
		wall->SetPos({ 50, 300 });
		wall->SetSize({ 100, 600 });
	}

	{
		Wall* wall = Object::CreateObject<Wall>();
		wall->SetPos({ 750, 300 });
		wall->SetSize({ 100, 600 });
	}
	
	{
		_player = Object::CreateObject<Player>();
		_player->SetPos({ 400, 300 });
	}
	
	{
		Monster* monster = Object::CreateObject<Monster>();
		monster->SetPos({ 450, 200 });
		monster->SetPlayer(_player);
		monster->SetFollowPlayer(false);
	}

	{
		Monster* monster = Object::CreateObject<Monster>();
		monster->SetPos({ 550, 200 });
		monster->SetPlayer(_player);
		monster->SetFollowPlayer(true);
	}

	{
		Monster* monster = Object::CreateObject<Monster>();
		monster->SetPos({ 350, 200 });
		monster->SetPlayer(_player);
		monster->SetFollowPlayer(true);
	}

	{
		Camera* camera = Object::CreateObject<Camera>();
		camera->SetPos({ GWinSizeX / 2, GWinSizeY / 2 });
		camera->SetPlayer(_player);
	}

	{
		UI::CreateUI<TestPanel>();
	}
	
}

void GameScene::Update()
{
	__super::Update();
}

void GameScene::Release()
{
	__super::Release();
}
