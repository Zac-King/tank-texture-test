#include <GL\glew.h>
#include <GL\wglew.h>
#include <GLFW\glfw3.h>

#include <cassert>
#include <fstream>
#include <string>
#include <sstream>

#include "crenderutils.h"

// unsigned integer
//typedef unsigned int GLuint;
//typedef float GLfloat;

#include <vector>
#include <string>
#include <fstream>

GLuint CreateShader(GLenum a_eShaderType, const char *a_strShaderFile)
{
	std::string strShaderCode;
	std::ifstream shaderStream(a_strShaderFile);

	if (shaderStream.is_open())
	{
		std::string Line = "";
		while (std::getline(shaderStream, Line))
		{
			strShaderCode += "\n" + Line;
		}
		shaderStream.close();
	}
	char const *szShaderSourcePointer = strShaderCode.c_str();
	GLuint uiShader = glCreateShader(a_eShaderType);
	glShaderSource(uiShader, 1, &szShaderSourcePointer, NULL);
	glCompileShader(uiShader);
	GLint iStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iStatus);
	if (iStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(uiShader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (a_eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return uiShader;
}

GLuint CreateProgram(const char *a_vertex, const char *a_frag)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, a_vertex));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, a_frag));
	GLuint uiProgram = glCreateProgram();

	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
		glAttachShader(uiProgram, *shader);

	glLinkProgram(uiProgram);

	GLint status;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(uiProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(uiProgram, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glDetachShader(uiProgram, *shader);
		glDeleteShader(*shader);
	}

	return uiProgram;
}


RenderData LoadMesh(Vertex *verts, unsigned nverts, Triangle *tris, unsigned ntris)
{
	RenderData ro;

	glGenBuffers(1, &ro.VBO);
	glGenBuffers(1, &ro.IBO);
	glGenVertexArrays(1, &ro.VAO);
	ro.size = 3 * ntris;

	glBindVertexArray(ro.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, ro.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ro.IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nverts, verts, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Triangle) * ntris, tris, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)  Vertex::eOffsets::ePosition);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)  Vertex::eOffsets::eColor);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)  Vertex::eOffsets::eUV);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return ro;
}

void drawRenderObject(RenderData ro,unsigned shader)
{
	glUseProgram(shader);
	glBindVertexArray(ro.VAO);
	glDrawElements(GL_TRIANGLES, ro.size, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glUseProgram(0);
}

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

TextureData LoadTexture(const char *a_path, int a_row = 1, int a_col = 1)
{
	TextureData td;
	////// Macros
	GLuint &handle = td.handle;
	int &width	   = td.width;
	int &height    = td.height;
	int &format    = td.format; // RGBA, RGB, Greyscale, etc.
	td.row		   = a_row;
	td.col		   = a_col;

	unsigned char* pixelData = stbi_load(a_path, &width, &height, &format, STBI_default);

	format = format == 3 ? GL_RGB : GL_RGBA;

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixelData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] pixelData;
	return td;
}