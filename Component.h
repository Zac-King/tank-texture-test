#pragma once

// Component type definition Macro
//#define compDef(typename) class typename : public ComponentBase<typename> { public:



// Component type constructor Macro
#define compMake(entity,varname) \
	(entity->varname = entity->varname->makeNew(entity))

#define compFree(entity,varname)\
		if (entity && entity->varname){			\
			entity->varname->e_owner = nullptr;	\
			entity->varname = nullptr; }		\

#include <vector>

struct Entity;

struct ComponentBase {};
//template<typename T> class ComponentBase\

#define compDef(typename) struct typename : public ComponentBase \
{																 \
public:															 \
	Entity *e_owner;										 	 \
	static std::vector<typename> &getData()						 \
		{														 \
				static std::vector<typename> d;					 \
				if (!d.size()) d.reserve(64);					 \
				return d;										 \
		}														 \
																 \
	static typename *makeNew(Entity *e)							 \
	{															 \
		for (int i = 0; i < getData().size(); ++i)				 \
			if (!getData()[i].e_owner)							 \
			{													 \
				getData()[i] = typename();						 \
				getData()[i].e_owner = e;						 \
				return &getData()[i];							 \
			}													 \
																 \
		getData().push_back(typename());						 \
		getData().back().e_owner = e;							 \
		return &getData().back();								 \
	}															 \


#define compEndDef };