#pragma once 
#include "SystemDef.h"
#include "Core.h"

sysDef(LifeTimeDynamics)

bool init() { return true; }
bool term() { return true; }

bool condition(Entity *e)
{
	return e->active && e->lifeTime;
}

void update(Entity *e)
{
	e->lifeTime->timer += core(Time).getDeltaTime();
		if (e->lifeTime->timer > e->lifeTime->totalLife)
		{
			core(Factory).FreeEntity(e);
		}
}
sysEndDef