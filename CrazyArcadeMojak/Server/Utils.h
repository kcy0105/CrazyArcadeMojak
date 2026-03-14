#pragma once
class Utils
{
public:
	static Vec2Int WorldToTile(Vec2 worldPos);
	static Vec2 TileToWorld(Vec2Int tilePos);
};