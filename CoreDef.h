#pragma once

// Use this to define a new core component
#define coreDef(typename) \
class typename : public Process \
{ public: \
static typename &instance() \
{ static typename t; return t; } \
protected:    \
typename() {} \

// Use this to end the definition
#define coreEndDef };

//Window::instance() == core(Window)
#define core(typename) typename::instance()



class Process
{
	friend class Engine;
	virtual bool init() = 0;
	virtual bool step() = 0;
	virtual bool term() = 0;
};