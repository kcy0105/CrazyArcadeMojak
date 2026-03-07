#pragma once

#pragma region COMMON
enum OBJECT_TYPE
{
	OBJECT_TYPE_PLAYER,
};

enum DIR
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

enum PLAYER_STATE
{
	PLAYER_STATE_IDLE,
	PLAYER_STATE_MOVE,
};

enum MAP_OBJECT_TYPE
{
	MAP_OBJECT_TYPE_NONE,
	MAP_OBJECT_TYPE_BREAKABLE_BLOCK,
	MAP_OBJECT_TYPE_SOLID_BLOCK,
	MAP_OBJECT_TYPE_WATER_BOMB,
};
#pragma endregion

enum class SceneType
{
	None,
	GameScene,
	DevScene,
	EditScene,
};

enum class ColliderType
{
	Box,
	Circle,
};