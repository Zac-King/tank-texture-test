#pragma once
#include "Component.h"
#include "ccollisionutils.h"
#include <list>

compDef(Collider)

int faction;
Shape shape;

std::list<Entity *> contacts;
 
Collider() : faction(0) {}

compEndDef