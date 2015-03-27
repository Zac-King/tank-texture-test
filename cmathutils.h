#pragma once
#include <cmath>

union vec2 { float v[2]; struct { float x, y; }; };
union vec3 { float v[3]; struct { float x, y, z; }; };
union vec4 { float v[4]; struct  { float x, y, z, w; }; vec3 xyz; };
union mat4 { float m[16]; float mm[4][4]; vec4 v[4]; };

typedef mat4 Matrix4;

vec3 vec3Zero();
mat4 mat4Identity();
mat4 mul(mat4 a, mat4 b);

float vec3Dot(vec3 a, vec3 b);
float vec3Length(vec3 a);
float vec3Distance(vec3 a, vec3 b);
float vec3AngleBetween(vec3 a, vec3 b);
float clamp(float min, float max, float value);
float snap(float min, float max, float value);

vec3 vec3Make(float x, float y, float z);
vec4 vec4Color(float x, float y, float z, float w );
mat4 getOrtho(float left, float right, float bottom, float top, float a_fNear, float a_fFar);

mat4 makeRotation(float angle);
mat4 makeTranslation(vec3 pos);
mat4 makeScale(vec3 scale);

mat4 operator*(mat4 a, mat4 b);
vec4 operator*(vec4 a, float b);
vec4 operator-(vec4 a, vec4 b);
vec4 operator+(vec4 a, vec4 b);

vec3 operator*(vec3 a, float b);
vec3 operator/(vec3 a, float b);
vec3 operator-(vec3 a, vec3 b);
vec3 operator+(vec3 a, vec3 b);

vec3 normal(vec3 a);

vec4 operator+(vec4 a, vec3 b);