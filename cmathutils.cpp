#include "cmathutils.h"

vec3 vec3Zero()
{
	vec3 r; r.x = r.y = r.z = 0; return r;
}
mat4 mat4Identity()
{
	const mat4 r = { 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	return r;
}
mat4 mul(mat4 a, mat4 b) // 4x3 * 3x2
{
	mat4 r = { 0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0 };

	for (int i = 0; i < 4; i++)			// rows
		for (int j = 0; j < 4; j++)		// cols
			for (int k = 0; k < 4; k++) // k shared dimension
				r.m[4 * i + j] += a.m[4 * i + k] * b.m[4 * k + j];

	return r;
}
////// 
float vec3Dot(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
float vec3Length(vec3 a)
{
	return sqrt(vec3Dot(a, a));
}
float vec3Distance(vec3 a, vec3 b)
{
	return vec3Length(a - b);
}
float vec3AngleBetween(vec3 a, vec3 b)
{
	return acos(vec3Dot(a, b)) / (vec3Length(a) * vec3Length(b));
}
	//float clamp(float min, float max, float value)
	//{
	//	return value < min ? min : value > max ? max : value;
	//}
float snap(float min, float max, float value)
{
	if (value < min || value > max) return value;

	return abs(value - min) < abs(value - max) ? min : max;
}
////// 
vec3 vec3Make(float x, float y, float z)
{
	vec3 a;
	a.x = x; a.y = y; a.z = z;
	return a;
}
vec4 vec4Color(float x, float y, float z, float w)
{
	vec4 a;
	a.x = x; a.y = y; a.z = z; a.w = w;
	return a;
}
mat4 getOrtho(float left, float right, float bottom, float top, float a_fNear, float a_fFar)
{
	//to correspond with mat4 in the shader
	//ideally this function would be part of your matrix class
	//however I wasn't willing to write your matrix class for you just to show you this
	//so here we are in array format!
	//add this to your matrix class as a challenge if you like!
	mat4 toReturn;
	toReturn.m[0] = 2.0 / (right - left);
	toReturn.m[1] = toReturn.m[2] = toReturn.m[3] = toReturn.m[4] = 0;
	toReturn.m[5] = 2.0 / (top - bottom);
	toReturn.m[6] = toReturn.m[7] = toReturn.m[8] = toReturn.m[9] = 0;
	toReturn.m[10] = 2.0 / (a_fFar - a_fNear);
	toReturn.m[11] = 0;
	toReturn.m[12] = -1 * ((right + left) / (right - left));
	toReturn.m[13] = -1 * ((top + bottom) / (top - bottom));
	toReturn.m[14] = -1 * ((a_fFar + a_fNear) / (a_fFar - a_fNear));
	toReturn.m[15] = 1;
	return toReturn;
}
////// 
mat4 makeRotation(float angle)
{
	mat4 r = { cos(angle), sin(angle), 0, 0,
		-sin(angle), cos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	return r;
}
mat4 makeTranslation(vec3 pos)
{
	mat4 r = mat4Identity();

	r.v[3].x = pos.x;
	r.v[3].y = pos.y;
	r.v[3].z = pos.z;

	return r;

}

mat4 makeScale(vec3 scale)
{
	mat4 r = mat4Identity();
	r.mm[0][0] = scale.x;
	r.mm[1][1] = scale.y;
	r.mm[2][2] = scale.z;
	return r;
}

////// 
vec4 operator*(vec4 a, float b)
{
	vec4 r = { a.x * b, a.y * b, a.z * b, a.w * b };
	return r;
}
vec4 operator-(vec4 a, vec4 b)
{
	vec4 r = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
	return r;
}
vec4 operator+(vec4 a, vec4 b)
{
	vec4 r = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
	return r;
}
////// 
vec4 operator+(vec4 a, vec3 b)
{
	vec4 r = { a.x + b.x, a.y + b.y, a.z + b.z, a.w };
	return r;
}
////// 
mat4 operator*(mat4 a, mat4 b)
{
	return mul(a, b);
}
////// 
vec3 operator*(vec3 a, float b)
{
	vec3 r = { a.x * b, a.y * b, a.z * b };
	return r;
}
vec3 operator/(vec3 a, float b)
{
	vec3 r = { a.x / b, a.y / b, a.z / b };
	return r;
}
vec3 operator-(vec3 a, vec3 b)
{
	vec3 r = { a.x - b.x, a.y - b.y, a.z - b.z };
	return r;
}
vec3 operator+(vec3 a, vec3 b)
{
	vec3 r = { a.x + b.x, a.y + b.y, a.z + b.z };
	return r;
}
////// 
vec3 normal(vec3 a)
{
	vec3 r = { a.x / vec3Length(a), a.y / vec3Length(a), a.z / vec3Length(a) };
	return r;
}