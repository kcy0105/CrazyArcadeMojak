#pragma once

#include "Protocol.pb.h"

enum PacketId
{
    S_EnterGame = 10,
    S_MyPlayer = 11,
    S_OtherPlayers = 12,
    S_PlayerLeave = 13,
    C_Move = 14,
    S_Move = 15,
    S_Tilemap = 16,
    C_WaterBomb = 17,
    S_WaterBomb = 18,
    S_Explode = 19,
    S_Dead = 20,
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
struct PacketIdType<Protocol::S_OtherPlayers>
{
    static const uint16 value = S_OtherPlayers;
};

template<>
struct PacketIdType<Protocol::S_PlayerLeave>
{
    static const uint16 value = S_PlayerLeave;
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

template<>
struct PacketIdType<Protocol::S_Explode>
{
    static const uint16 value = S_Explode;
};

template<>
struct PacketIdType<Protocol::S_Dead>
{
    static const uint16 value = S_Dead;
};

