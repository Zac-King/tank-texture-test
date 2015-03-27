#pragma once
#include "Component.h"

compDef(Render)

std::string shader, texture, mesh;
float frame, frameRate;

Render() : frame(0), frameRate(12) {}

compEndDef