#pragma once

#include "cmathutils.h"
#include "Component.h"

compDef(Rigidbody)

	vec3 velocity, force;
	float drag, mass, radius;
	float angularMomentum, torque;

	Rigidbody() : velocity(vec3Zero()),
				  force(vec3Zero()),
				  drag(0), mass(1), radius(100),
				  angularMomentum(0), torque(0) {}

	void addForce(vec3 const &a_force) {  force = force + a_force;  }
	void addTorque(float a_torque)  { torque = torque + a_torque; }

compEndDef