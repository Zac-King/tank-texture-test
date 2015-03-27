#pragma once
#include "CoreDef.h"
#include "Core.h"

#define sysDef(typename) \
	class typename : public System {


#define sysEndDef };


struct Entity;
class System : public Process
{
	bool step()
	{
		for (int i = 0; i < core(Factory).getEntities().size(); i++)
			if (condition(&core(Factory).getEntities()[i]))
				update(&core(Factory).getEntities()[i]);
		return true;
	}
	virtual bool condition(Entity *e) = 0;
	virtual void update(Entity *e) = 0;
};