#pragma once
#define NOMINMAX
#include <windows.h>

#include <vector>
#include <algorithm>
#include <format>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <functional>
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