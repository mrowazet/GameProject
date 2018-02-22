#pragma once

namespace engine
{
	struct Entity;
	struct ComponentBase;
	class System;

	using Id = unsigned int;
	using EntityId = Id;

	using PoolSize = unsigned int;

	using ComponentFlags = unsigned int;
	using ComponentPtr = ComponentBase*;

	using s32 = signed int;
	using u32 = unsigned int;
};

namespace engine
{
namespace core
{

}
}