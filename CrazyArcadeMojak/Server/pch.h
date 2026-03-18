#pragma once

#include "CorePch.h"

#include "Enums.h"
#include "Types.h"
#include "Values.h"
#include "Utils.h"

#include "Protocol.pb.h"
#include "Struct.pb.h"

#include "format"

#ifdef _DEBUG
#pragma comment(lib, "Network\\Debug\\Network.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "Network\\Release\\Network.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "ServerPacketHandler.h"

#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

#define GET_SINGLE(classname)	classname::GetInstance()