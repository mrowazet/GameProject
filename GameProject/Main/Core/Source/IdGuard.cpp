#include "IdGuard.h"

namespace engine
{

IdGuard::IdGuard(Id p_maxId)
	:m_maxId(p_maxId)
{
}

Id IdGuard::getNextId()
{
	return UNDEFINED_ENTITY_ID;
}

void IdGuard::freeId(Id p_id)
{

}

}