#pragma once
#include "SystemDef.h"


sysDef(DynamicCollisionResponse)

bool term() { return true; }
bool init() { return true; }

bool condition(Entity *e)
{
	return e->rigidbody && e->collider && e->active;
}


void update(Entity *e)
{
	
	for each(auto o in e->collider->contacts)
		if (condition(o) && e->factoryIndex < o->factoryIndex)
		{
			auto &v1 = e->rigidbody->velocity; 
			auto &v2 = o->rigidbody->velocity; 
			auto &m1 = e->rigidbody->mass;
			auto &m2 = o->rigidbody->mass;

			
			////solve for v1' v2'

			vec3 v1f, v2f;
			v2f = ((v1 * -2 * m2) + v2*(m1 - m2)) * (1 / (-m2 - m1));
			v1f = v1 + v2 - v2f;

			v1 = v1f;
			v2 = v2f;
		}

}




sysEndDef