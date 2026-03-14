#pragma once

#include "Protocol.pb.h"

enum PacketId
{
    S_EnterGame = 10,
    S_MyPlayer = 11,
    S_AddObject = 12,
    S_RemoveObject = 13,
    C_Move = 14,
    S_Move = 15,
    S_Tilemap = 16,
    C_WaterBomb = 17,
    S_WaterBomb = 18,
};

template<typename T>
struct PacketIdType
{
};

template<>
struct PacketIdType<Protocol::S_EnterGame>
{
    static const uint16 value = S_EnterGame;
};

template<>
struct PacketIdType<Protocol::S_MyPlayer>
{
    static const uint16 value = S_MyPlayer;
};

template<>
struct PacketIdType<Protocol::S_AddObject>
{
    static const uint16 value = S_AddObject;
};

template<>
struct PacketIdType<Protocol::S_RemoveObject>
{
    static const uint16 value = S_RemoveObject;
};

template<>
struct PacketIdType<Protocol::C_Move>
{
    static const uint16 value = C_Move;
};

template<>
struct PacketIdType<Protocol::S_Move>
{
    static const uint16 value = S_Move;
};

template<>
struct PacketIdType<Protocol::S_Tilemap>
{
    static const uint16 value = S_Tilemap;
};

template<>
struct PacketIdType<Protocol::C_WaterBomb>
{
    static const uint16 value = C_WaterBomb;
};

template<>
struct PacketIdType<Protocol::S_WaterBomb>
{
    static const uint16 value = S_WaterBomb;
};

