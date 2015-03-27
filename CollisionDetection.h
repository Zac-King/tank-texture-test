#pragma once 
#include "SystemDef.h"
#include "Core.h"
#include "ccollisionutils.h"

sysDef(CollisionDetection)

bool init() { return true; }
bool term() { return true; }

bool condition(Entity *e)
{
	return e->active && e->transform && e->collider;
}

bool isCollidingFunction(Entity *a, Entity *b)
{
	a->collider->shape.orgin = a->transform->globalPosition();
	b->collider->shape.orgin = b->transform->globalPosition();
	return ShapeVsShape(a->collider->shape, b->collider->shape) >= 0;
}

void kill(Entity *e)
{
	core(Factory).FreeEntity(e);
	if (e->dependent)
		core(Factory).FreeEntity(e->dependent->child);
}

void update(Entity *e)
{
	auto &ents = core(Factory).getEntities();
	for (int i = e->factoryIndex; i < ents.size(); ++i)
		if (condition(&ents[i]) &&
			e->collider->faction != ents[i].collider->faction&&
			isCollidingFunction(e, &ents[i]))
			{	
				e->collider->contacts.push_back(&ents[i]);
				(&ents[i])->collider->contacts.push_back(e);
			}
}

sysEndDef