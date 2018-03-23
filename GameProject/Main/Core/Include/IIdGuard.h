#pragma once
#include "Types.h"
#include "Constants.h"

namespace engine
{

class IIdGuard
{
public:
	IIdGuard() = default;
	virtual ~IIdGuard() = default;

	virtual Id getNextId() = 0;
	virtual void freeId(Id p_id) = 0;
	virtual void reset() = 0;
};

}