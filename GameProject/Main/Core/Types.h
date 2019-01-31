#pragma once
#include <iso646.h>
#include <bitset>

namespace engine
{
namespace core
{

}

namespace CoreParams
{
	constexpr unsigned int MAX_NR_OF_COMPONENTS = 32u;
}
}

namespace engine
{
	struct Entity;
	struct ComponentBase;
	class System;

	using Id = unsigned int;
	using EntityId = Id;

	using PoolSize = unsigned int;

	using ComponentFlags = std::bitset<CoreParams::MAX_NR_OF_COMPONENTS>;
	using ComponentPtr = ComponentBase*;

	using s8 = signed char;
	using u8 = unsigned char;

	using s32 = signed int;
	using u32 = unsigned int;

	using ComponentIndex = u8;
};