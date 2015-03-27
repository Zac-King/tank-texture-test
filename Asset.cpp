#include "Asset.h"
#include "Camera.h"

void Asset::drawLine(vec3 a, vec3 b, vec4 color)
{
	auto &camera = Camera::instance().getCamera();
	
	glLoadIdentity();
	glMultMatrixf(camera.projection.m);
	glMultMatrixf(camera.view.m);

	glBegin(GL_LINE_LOOP);
		glColor4fv(color.v);
		glVertex3fv(a.v);
		glVertex3fv(b.v);
	glEnd();
}

void Asset::drawRect(Shape a, vec4 color)
{
	vec3 center;
	center = vec3Make(a.rect.position.x, a.rect.position.y, 0) ;

	vec3 TL, TR, BL, BR;
	BL = a.rect.min + center;
	BR = vec3Make(a.rect.max.x, a.rect.min.y, 0) + center;
	TL = vec3Make(a.rect.min.x, a.rect.max.y, 0) + center;
	TR = a.rect.max + center;

	drawLine(center, TL, color); //yellow
	//drawLine(center, TR, color); //white
	//drawLine(center, BL, color); //Light Blue
	//drawLine(center, BR, color); //blue

	drawLine(TL, TR, color); //yellow
	drawLine(TL, BL, color); //yellow
	drawLine(TR, BR, color); //white
	drawLine(BL, BR, color); //Light Blue
}

#define PI 3.14159265359

void Asset::drawCircle(Shape a, vec4 color)
{
	vec3 center;
	center = vec3Make(a.circle.position.x, a.circle.position.y, 0);

	vec3 nextPos, oldPos;
	oldPos = a.circle.position + vec3Make(a.circle.radius, 0, 0);
	for (float i = -PI/12; i <= 2*PI; i+= PI/12)
	{
		float x = a.circle.radius * cos(i);
		float y = a.circle.radius * sin(i);
		nextPos = center + vec3Make(x, y, 0);
		drawLine(nextPos, oldPos, color);
		oldPos = nextPos;
	}
}

void Asset::drawRay(Shape a, vec4 color)
{
	vec3 orgin;
	orgin = vec3Make(a.ray.position.x, a.ray.position.y, 0);

	vec3 end;
	end = orgin + a.ray.normal * 8;
	drawLine(orgin, end, color);
	drawCircle(MakeCircle(orgin.x, orgin.y, 7), color);
}

void Asset::drawPlane(Shape a, vec4 color)
{
	vec3 Pos = vec3Make(a.plane.position.x, a.plane.position.y, 0);

	float angle = atan2f(a.plane.normal.x, a.plane.normal.y) + PI / 2;

	vec3 right;
	right.x = cos(angle);
	right.y = sin(angle);
	right.z = 0;
	
	drawLine(Pos - right * 24, Pos + right * 24, color);
	drawLine(Pos, Pos + a.plane.normal * 8, color);
	drawCircle(MakeCircle(Pos.x, Pos.y, 3), color);
}

void Asset::drawShape(Shape a, vec4 color)
{
	a.position = a.orgin + a.position;
	
	if (a.type == Shape::eCircle)
	{
		return drawCircle(a, color);
	}
	if (a.type == Shape::eRect)
	{
		return drawRect(a, color);
	}
	if (a.type == Shape::eRay)
	{
		return drawRay(a, color);
	}
	if (a.type == Shape::ePlane)
	{
		return drawPlane(a, color);
	}
}