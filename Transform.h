#pragma once
#include "Component.h"
#include "cmathutils.h"

compDef(Transform)

	Transform *m_parent;
	mat4 m_local, m_global;

	vec3 &right()   { return m_local.v[0].xyz; }
	vec3 &up()      { return m_local.v[1].xyz; }
	vec3 &forward() { return m_local.v[2].xyz; }
	vec3 &position(){ return m_local.v[3].xyz; }
	vec3 globalPosition() { return m_global.v[3].xyz; }

	Transform() :
		m_parent(nullptr),
		m_local(mat4Identity()),
		m_global(mat4Identity())  { }

	mat4 getGlobal()
	{
		return m_global = (m_parent ? m_local * m_parent->getGlobal() : m_local);
	}

compEndDef