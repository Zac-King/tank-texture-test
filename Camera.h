#pragma once

#include "CoreDef.h"
#include "cmathutils.h"
#include "Window.h"

struct CameraData
{
	mat4 view, projection;
};

coreDef(Camera)

	CameraData MainCamera;

	bool init()
	{
		MainCamera.view = mat4Identity();
		MainCamera.projection = getOrtho(0, core(Window).width(), 0, core(Window).height(), 0, 100);
		return true;
	}

	bool step() { return true; }
	bool term() { return true; }

	public:
		CameraData &getCamera() { return MainCamera; }

coreEndDef


