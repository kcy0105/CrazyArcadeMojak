#pragma once
#define NOMINMAX
//#include <windows.h>

#include "CorePch.h"

#include <format>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

#include "Types.h"
#include "Define.h"
#include "Enums.h"
#include "Utils.h"
#include "Values.h"

#pragma comment(lib, "msimg32.lib")

#define _CRTDEBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif
