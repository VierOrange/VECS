#pragma once
#include <cstdint>
#include <bitset>
#include <cassert>

#define MAX_ENTITIES 5000
#define MAX_COMPONENTS 32

using Entity = std::uint32_t;
using ComponentType = std::uint8_t;
using Signature = std::bitset<MAX_COMPONENTS>;
