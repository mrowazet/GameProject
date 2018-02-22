#pragma once
#include "IIdGuard.h"

namespace engine
{

class IdGuard : public IIdGuard
{
public:
	IdGuard(Id p_maxId);

	Id getNextId() override;
	void freeId(Id p_id) override;

private:
	const Id m_maxId;

};

}
