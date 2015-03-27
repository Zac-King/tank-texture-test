#include "ccollisionutils.h"
#include <cassert>
#include <iostream>
////Helper function
float clamp(float min, float max, float value)
	{	return value < min ? min : value > max ? max : value;	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float MIN(float a, float b) { return a < b ? b : a; }
float MAX(float a, float b) { return a < b ? a : b; }
////////////////////////////////////////////////////////////////
float ShapeVsShape(Shape  a, Shape b)
{
	a.position = a.position + a.orgin;
	b.position = b.position + b.orgin;

	if (a.type == Shape::eCircle && b.type == Shape::eCircle)
		return CircleVsCircle(a.circle, b.circle);
	
	if (a.type == Shape::eRect && b.type == Shape::eRect)
		return RectVsRect(a.rect, b.rect);
	
	if (a.type == Shape::eRay && b.type == Shape::ePlane)
		return RayVsPlane(a.ray, b.plane);
	if (a.type == Shape::ePlane && b.type == Shape::eRay)
		return RayVsPlane(a.ray, b.plane);

	if (a.type == Shape::eCircle && b.type == Shape::ePlane)
		return CircleVsPlane(a.circle, b.plane);
	if (a.type == Shape::ePlane && b.type == Shape::eCircle)
		return CircleVsPlane(b.circle, a.plane);

	if (a.type == Shape::eCircle && b.type == Shape::eRect)
		return CircleVsRect(a.circle, b.rect);
	
	if (a.type == Shape::eRect && b.type == Shape::eCircle)
		return CircleVsRect(a.rect, b.circle);
	
	if (a.type == Shape::eCircle && b.type == Shape::eRay)
		return CircleVsRay(a.circle, b.ray);
	if (a.type == Shape::eRect && b.type == Shape::eRay)
		return RectVsRay(a.rect, b.ray);
	
	if (a.type == Shape::eRect && b.type == Shape::ePlane)
		return RectVsPlane(a.rect, b.plane);
	if (a.type == Shape::ePlane && b.type == Shape::eRect)
		return RectVsPlane(b.rect, a.plane);

	return -1;
}
////////////////////////////////////////////////////////////////
float CircleVsCircle(const Circle &a, const Circle &b)
{
	return (a.radius + b.radius) - vec3Distance(a.position,b.position);
}
////////////////////////////////////////////////////////////////
float RectVsRect( Rect a,  Rect b)
{
	a.max = a.max + a.position;
	a.min = a.min + a.position;

	b.max = b.max + b.position;
	b.min = b.min + b.position;

	if (a.min.x > b.max.x || b.min.x > a.max.x)
		return -1;
	if (a.min.y > b.max.y || b.min.y > a.max.y)
		return -1;

	float diffx1 = abs(a.min.x - b.max.x);
	float diffx2 = abs(b.min.x - a.max.x);

	float diffy1 = abs(a.min.y - b.max.y);
	float diffy2 = abs(b.min.y - a.max.y);

	return ((diffx1 < diffx2 ? diffx1 : diffx2) +
		    (diffy1 < diffy2 ? diffy1 : diffy2)) / 2;
}
////////////////////////////////////////////////////////////////
float PointVsPlane(const Point &o, const Plane &l)
{
	return vec3Dot(l.normal,(o - l.position));
}
////////////////////////////////////////////////////////////////
float RayVsPlane(const Plane &p, const Ray  &r) { return RayVsPlane(r, p); }
float RayVsPlane(const Ray &r, const Plane &p)
{
	return -PointVsPlane(r.position, p) / vec3Dot(p.normal, r.normal * r.length);
}
////////////////////////////////////////////////////////////////
float CircleVsPlane(const Plane &p, const Circle  &c) { return CircleVsPlane(c, p); }
float CircleVsPlane(const Circle &c, const Plane  &p)
{
	//distance between plane and circle
	return c.radius - PointVsPlane(c.position, p);
}
////////////////////////////////////////////////////////////////
float CircleVsRect( const Rect &r, const Circle  &c) { return CircleVsRect(c, r); }
float CircleVsRect(const Circle &c,  const Rect  &r)
{
	vec3 rc = (r.min + r.max) * (0.5f) + r.position; // calculating the center of the rectangle
	vec3 dim = (r.max - r.min) * (0.5f);			 // dimensions

	Point p;
	Point pc = (c.position - rc); // looks good!
	
	p.x = clamp(-dim.x, dim.x, pc.x);
	p.y = clamp(-dim.y, dim.y, pc.y);
	p.z = 0;

	std::cout << pc.x << " " << pc.y << " " << std::endl;
	std::cout << p.x << " " << p.y << " " << std::endl;
	
	float t = c.radius - vec3Length(pc - p);
	std::cout << t << std::endl;
	if (t == c.radius)
	{
		vec3 minDiff = p - r.min;
		vec3 maxDiff = r.max - p;
		if (vec3Length(maxDiff) < vec3Length(minDiff))
			t = maxDiff.x < maxDiff.y ? maxDiff.x : maxDiff.y;
		else
			t = minDiff.x < minDiff.y ? minDiff.x : minDiff.y;
		return t;
	}
	return t;
}
////////////////////////////////////////////////////////////////
float CircleVsRay(const Ray &r, const Circle  &c) { return CircleVsRay(c, r); }
float CircleVsRay(const Circle &c, const Ray    &r)
{
	vec3 C = c.position - r.position;
	vec3 A = r.normal*r.length;
	float infinity = vec3Length(C) + c.radius;

	float t = c.radius - vec3Distance(A, C);
	if (t > 0)
		return t;

	if (t < 0 && vec3Dot(A, C) > vec3Length(C) - c.radius)
	{
		float b = sqrt((vec3Length(C) * vec3Length(C)) + (vec3Dot(A, C) * vec3Dot(A, C)));
		t = c.radius - b;

		return t;
	}
}
////////////////////////////////////////////////////////////////
float RectVsPlane(const Plane &p,  Rect r) { return RectVsPlane(r, p); }
float RectVsPlane( Rect r, const Plane &p)
{
	auto topLeft  = r.min + r.position - p.position;
	auto topRight = vec3Make(r.max.x, r.min.y, 0) + r.position - p.position;
	auto botLeft = vec3Make(r.min.x, r.max.y, 0) + r.position - p.position;
	auto botRight = r.max + r.position - p.position;

	float ta = vec3Dot(topLeft, p.normal);
	float tb = vec3Dot(botRight, p.normal);
	float tc = vec3Dot(botLeft, p.normal);
	float td = vec3Dot(topRight, p.normal);

	if (ta > 0 && tb > 0 && tc > 0 && td > 0) /*||(ta < 0 && tb < 0 && tc < 0 && td < 0))*/
	{
		return -1; //miss
	}

	return 1; //hit
}
////////////////////////////////////////////////////////////////
float RectVsRay(const Ray &r, const Rect  &e) { return RectVsRay(e, r); }
float RectVsRay( Rect   e, const Ray    &r)
{
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Shape MakeCircle(float x, float y, float r)
{
	Shape a;
	a.type = Shape::eCircle;
	a.circle.position.x = x;
	a.circle.position.y = y;
	a.circle.position.z = 0;
	a.circle.radius = r;
	return a;
}

Shape MakeRect(float x, float y, float xMin, float yMax, float xMax, float yMin)
{
	Shape a;
	a.type = Shape::eRect;
	a.rect.position = vec3Make(x, y, 0);
	a.rect.min = vec3Make(xMin, yMin, 0);
	a.rect.max = vec3Make(xMax, yMax, 0);
	return a;
}

Shape MakePlane(float x, float y, vec3 n)
{
	Shape a;
	a.type = Shape::ePlane;
	a.plane.position.x = x;
	a.plane.position.y = y;
	a.plane.position.z = 0;
	a.plane.normal = n;
	return a;
}

Shape MakeRay(float x, float y)
{
	Shape a;
	a.type = Shape::eRay;
	a.ray.position.x = x;
	a.ray.position.y = y;
	a.ray.position.z = 0;
	a.ray.normal = normal(a.ray.position);
	a.ray.length = vec3Length(a.ray.normal);
	return a;
}

vec3 AABBNormal(const vec3 &mina, const vec3 &maxa, const vec3 &minb, const vec3 &maxb)
{
	static const vec3 faces[4] = { { -1, 0, 0 }, { 1, 0, 0 }, { 0, -1, 0 }, { 0, 1, 0 } };

	float distances[4] = { (maxb.x - mina.x), (maxa.x - minb.x), (maxb.y - mina.y), (maxa.y - minb.y) };


	float deepestPenetration;
	vec3 normal;
	for (int i = 0; i < 4; ++i)
		if ((i == 0) || (distances[i] < deepestPenetration))
		{
			normal = faces[i];
			deepestPenetration = distances[i];

		}

	return normal;
}

//void runCollisionTest()
//{
//	assert(ShapeVsShape( MakeCircle(0,0,1),  MakeCircle(1,0,1)) > 0);
//		//assert(ShapeVsShape(MakeCircle(0, 0, 1), MakeCircle(1, 0, 1)) < 0);
//	assert(ShapeVsShape(MakeCircle(1, 0, 1), MakeRect(1, 0, 1, 0)) < 0);
//		//assert(ShapeVsShape(MakeCircle(1, 0, 1), MakeRect(1, 0, 1, 0)) > 0);
//	assert(ShapeVsShape( MakeCircle(1,0,1),     MakePlane(1,0)) > 0);
//		//assert(ShapeVsShape( MakeCircle(1,0,1),     MakePlane(1,0,1,1)) < 0);
//	assert(ShapeVsShape( MakeCircle(1,0,1), MakeRay(1,0)) > 0);
//		//assert(ShapeVsShape( MakeCircle(1,0,1), MakeRay(2,2,1,1,2)) > 0);
//
//	assert(ShapeVsShape( MakeRect(0,1,0,1),  MakeRect(1,0,1,0)) < 0);
//		//assert(ShapeVsShape(MakeRect(0, 1, 0, 1), MakeRect(1, 0, 1, 0)) > 0);
//	assert(ShapeVsShape(MakeRect(0, 1, 0,1), MakePlane(1, 0)) > 0);
//		//assert(ShapeVsShape(MakeRect(0, 1, 0, 1), MakePlane(1, 0, 1, 1)) < 0);
//	//assert(ShapeVsShape(MakeRect(), MakeRay()));
//
//	//assert(ShapeVsShape(MakeRay(2,2,1,1,3),     MakePlane(2,2,1,1)) < 0);
//}