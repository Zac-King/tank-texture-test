#pragma once
#include <GL\glew.h>
#include <GL\wglew.h>
#include <GLFW\glfw3.h>
#include "cmathutils.h"

#include <cassert>
#include <fstream>
#include <string>
#include <sstream>

// don't use these.
// Make your own. These are for demonstration only.

//mat4 mul(mat4 a, vec4 b);
/////////////////////////////////////////////////////////




struct Vertex
{
	enum eOffsets { ePosition = 0, eColor = 16, eUV = 32, eSize = 40 };
	vec4 position;
	vec4 color;
	vec2 uv;
}; //

struct Triangle		{ unsigned v[3]; }; // 12 bytes

struct RenderData	{ unsigned VBO, IBO, VAO, size; };

struct TextureData	{ int width, height, format; GLuint handle; int row, col; };

typedef GLuint Shader;

void drawRenderObject(RenderData ro, unsigned shader);

RenderData	LoadMesh(Vertex *verts, unsigned nverts, Triangle *tris, unsigned ntris);
Shader		CreateProgram(const char *a_vertex, const char *a_frag);
TextureData LoadTexture(const char *a_path, int a_row, int a_col);



