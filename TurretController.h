#pragma once
#include "Controller.h"

compDef(Turret)

char rotate_left, rotate_right, fire;

float speed, torque;

Turret(Controller &parent) : speed(1), torque(1), rotate_left('Q'), rotate_right('E'), fire(' ') {}

compEndDef