#pragma once

enum class SceneType
{
	None,
	GameScene,
	DevScene,
};

enum class ColliderType
{
	Box,
	Circle,
};

using Dir = Protocol::DIR;

using PlayerState = Protocol::PLAYER_STATE;