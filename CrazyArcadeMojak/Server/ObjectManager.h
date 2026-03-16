#pragma once
class ObjectManager
{

private:
	unordered_map<uint64, PlayerRef> _players;
	unordered_map<uint64, WaterBombRef> _bombs;
};

