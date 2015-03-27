#pragma once
#include <vector>
#include "CoreDef.h"

#define initCore() Engine::init()
#define runGame()  while (Engine::step()); Engine::term()
#define coreLoad(typename) Engine::addCore(&core(typename))
#define systemLoad(typename) Engine::addSystem(&typename())

class Engine
{
	std::vector<Process*> m_cores;
	std::vector<Process*> m_systems;
	Engine() {}
	static Engine &instance() { static Engine e; return e; }

public:

	static void addCore(Process *a_core) { instance().m_cores.push_back(a_core); }
	static void addSystem(Process *a_system) { instance().m_cores.push_back(a_system); }
	static bool init()
	{
		bool r = true;
		for each(Process *core in instance().m_cores)
			r &= core->init();
		for each(Process *system in instance().m_systems)
			r &= system->init();
		return r;
	}
	static bool step()
	{	
		bool r = true;
		for each(Process *core in instance().m_cores)
			r &= core->step();

		for each(Process *system in instance().m_systems)
			r &= system->step();
		return r;
	}
	static bool term()
	{
		bool r = true;
		for each(Process *core in instance().m_cores)
			r &= core->term();
		for each(Process *system in instance().m_systems)
			r &= system->term();
		return r;
	}
};