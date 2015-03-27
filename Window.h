#pragma once

#pragma once
#include "CoreDef.h"
#include <GL\glew.h>
#include <GL\wglew.h>
#include <GLFW\glfw3.h>


#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 1080
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 720
#endif

#ifndef WINDOW_NAME
#define WINDOW_NAME "hellOpenGL"
#endif


coreDef(Window)

	GLFWwindow *m_window;

	bool init()
	{
		if (!glfwInit()) return false;

		m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
		if (!m_window) { glfwTerminate(); return false; }
		glfwMakeContextCurrent(m_window);
		if (glewInit() != GLEW_OK) { glfwTerminate(); return false; }

		return true;
	}
	bool step()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return !glfwWindowShouldClose(m_window);
	}
	bool term()
	{
		glfwTerminate();
		return true;
	}

public:

	int width()    { return WINDOW_WIDTH; }
	int height()   { return WINDOW_HEIGHT; }
	GLFWwindow *getHandle() { return m_window; }

coreEndDef