#version 330

in vec2 UV;
in vec4 vertColor;

out vec4 outputColor;

void main()
{
	outputColor =  vertColor;
}