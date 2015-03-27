#pragma once
#include "CoreDef.h"
#include "GLFW\glfw3.h"
#include "Window.h"

coreDef(Time)

float m_deltaTime, m_totalTime;

bool step()
{
	float currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_totalTime;
	m_totalTime = currentTime;
	return true;
}
bool term() { return true;  }
bool init() { m_deltaTime = m_totalTime = 0; glfwSetTime(0); return true; }

public:
	float getDeltaTime() const { return m_deltaTime; }

coreEndDef