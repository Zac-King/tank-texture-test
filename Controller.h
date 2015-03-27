#pragma once 
#include "Component.h"

compDef(Controller)

char  forward, reverse, turn_Left, turn_Right, fire;

float speed, torque;

float firingDelay;
float firingTimer;

void clear()
{
	forward = reverse = turn_Left = turn_Right = fire = 0;
}

Controller() : speed(100), torque(1), firingDelay(1), firingTimer(0),
turn_Left('A'), turn_Right('D'), forward('W'), reverse('S'), fire('C')
{}


compEndDef
