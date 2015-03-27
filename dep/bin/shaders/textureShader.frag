#version 150

in vec4 vertColor;
in vec2 UV;

out vec4 outputColor;

uniform sampler2D Diffuse;
uniform int rows;
uniform int cols;
uniform int frame;

void main()
{
	
	outputColor = texture(Diffuse,UV).rgba * vertColor;

	//UV(BL(0,0), BR(1, 0), TR(1, 1), TL(0,1));

	int c = frame % cols;
	int r = frame / cols;

	float u = c / (cols * 1.0f);
	float v = r / (rows * 1.0f); 

	vec2 cUV = vec2(u,v) + UV * vec2(1.f/cols, 1.f/rows);

	outputColor = texture(Diffuse,cUV).rgba;

	
	if(outputColor.rgb == vec3(1,1,1))
	{

			outputColor.a = 0;
		}

	//outputColor = vec4(cUV,0,1);
	//outputColour.a = 1;
	//outputColour = vec4(1,1,1,1);
}