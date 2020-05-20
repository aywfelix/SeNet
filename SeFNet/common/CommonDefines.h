#pragma once

#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <sys/stat.h>
#include <sys/types.h>
#include <cassert>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>
#include <thread>
#include <functional>
#include <memory>


using ushort8 = uint8_t;
using short8 = int8_t;
using sint32 = int32_t;
using uint32 = uint32_t;
using sing64 = int64_t;
using uint64 = uint64_t;
using time_t = int64_t;

#define TRUE 1
#define FALSE 0

#define NOMINMAX
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

static const char* TABLE_PATH = "../DataTable/";
static const char* SERVER_CFG = "../Config/ServerConf.json";
static constexpr sint32 GAME_MAX_PLAYERS = 5000;
static constexpr sint32 LOOP_TIMEOUT = 30; // event loop 30ms

#ifndef DELETE_PTR
#define DELETE_PTR(x) if((x)) { delete (x); (x)=nullptr;}
#endif

#ifndef DELETE_PTR_ARR
#define DELETE_PTR_ARR(x) if((x)) { delete[] (x); (x)=nullptr;}
#endif

bool IsEqualFloat(const float fval, float epsilon);
bool IsEqualDouble(const double dval, double epsilon);