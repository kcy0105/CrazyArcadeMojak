#pragma once

#include "CorePch.h"

#ifdef _DEBUG
#pragma comment(lib, "Network\\Debug\\Network.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "Network\\Release\\Network.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif