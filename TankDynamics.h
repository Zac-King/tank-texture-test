#pragma once 
#include "SystemDef.h"
#include "Core.h"
sysDef(TankDynamics)

bool init() { return true; }
bool term() { return true; }

bool condition(Entity *e)
{
	return e && e->active && e->rigidbody && e->transform && e->controller;
}

void update(Entity *e)
{

	float dt = core(Time).getDeltaTime();

	if (core(Input).getKeyDown(e->controller->forward))
		{	e->rigidbody->addForce(e->transform->up() * e->controller->speed);	}
	if (core(Input).getKeyDown(e->controller->reverse))
		{	e->rigidbody->addForce(e->transform->up() * (e->controller->speed * -1));	}
	if (core(Input).getKeyDown(e->controller->turn_Left))
		{	e->rigidbody->addTorque(e->controller->torque * -1);}
	if (core(Input).getKeyDown(e->controller->turn_Right))
		{	e->rigidbody->addTorque(e->controller->torque * 1);	}
	//Bullet
	auto &ref = e->controller;
	if (core(Input).getKeyDown(ref->fire) && ref->firingDelay < ref->firingTimer)
	{
		core(Factory).MakeBullet(e->transform, e->collider->faction ,800);
		ref->firingTimer = 0;
	}
	else ref->firingTimer += core(Time).getDeltaTime();
}
sysEndDef