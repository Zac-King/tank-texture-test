#pragma once

#include "SystemDef.h"

sysDef(DependentClear)

bool init() { return true; }
bool term() { return true; }

bool condition(Entity *e)
{
	return !e->active && e->dependent && e->dependent->child->active;
}

void update(Entity *e)
{
	core(Factory).FreeEntity(e->dependent->child);
}


sysEndDef