#pragma once
#include "cmathutils.h"

struct Circle { vec3 position; float radius; };
struct Rect { vec3 position; vec3 min, max; };
struct Plane { vec3 position, normal; };
struct Ray{ vec3 position, normal; float length; };

typedef vec3 Point;

struct Shape
{
	enum Type {eCircle, eRect, ePlane, eRay, ePoint, eNone} type;
	vec3 orgin;

	union
	{
		vec3 position;
		Circle circle;
		Rect rect;
		Plane plane;
		Ray ray;
		Point point;
	};
};

float ShapeVsShape  (Shape a, Shape b);

float CircleVsCircle(const Circle &a, const Circle &b);
float CircleVsPlane(const Circle &c, const Plane  &p);
float CircleVsRect(const Circle &c, const  Rect   &r);
float CircleVsRay(const Circle &c, const Ray    &r);
float CircleVsRect(const Rect &r ,const Circle  &c );
float RectVsRect( Rect   a,  Rect  b);
float RectVsPlane( Rect r, const Plane &p);
float RectVsRay( Rect   e, const Ray    &r);
float RayVsPlane(const Ray    &r, const Plane  &p);

Shape MakeCircle(float x, float y, float r);
Shape MakeRect(float x, float y, float xMin, float yMax, float xMax, float yMin);
Shape MakePlane(float x, float y, vec3 n);
Shape MakeRay(float x, float y);

vec3 AABBNormal(const vec3 &mina, const vec3 &maxa, const vec3 &minb, const vec3 &maxb);

void runCollisionTest();