#version 330

layout(location = 0)in vec4 Position;
layout(location = 1)in vec4 Color;
layout(location = 2)in vec2 vertexUV;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec4 vertColor;
out vec2 UV;

void main()
{
	vertColor = Color;
    UV = vertexUV;

	gl_Position = Projection * View * Model * Position;
}