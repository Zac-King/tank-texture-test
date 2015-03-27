#pragma once 
#include "SystemDef.h"

sysDef(RigidbodyDynamics)

bool init() { return true; }
bool term() { return true; }

bool condition(Entity *e) 
{
	return e && e->active && e->rigidbody && e->transform;
}

void update(Entity *e) 
{
	auto &pos		= e->transform->position();
	auto &rot		= e->transform->m_local;
	auto &vel		= e->rigidbody->velocity;
	auto &force		= e->rigidbody->force;
	auto &drag		= e->rigidbody->drag;
	auto &mass		= e->rigidbody->mass;
	auto &aM		= e->rigidbody->angularMomentum;
	auto &tor		= e->rigidbody->torque;
	float dt		= core(Time).getDeltaTime();

	e->transform->m_local = makeRotation(e->rigidbody->angularMomentum * -1 * dt) * e->transform->m_local * makeTranslation(e->rigidbody->velocity * dt);

	float a_damp = (aM * -1) * drag;
	float a_acc = (tor + a_damp) * (1 / mass);

	vec3 damp = (vel * -1) * drag;
	vec3 acc = (force + damp) * (1 / mass);

	vel = vel + acc * dt;
	force = vec3Zero();

	aM = aM + a_acc * dt;
	tor = 0;
}

sysEndDef