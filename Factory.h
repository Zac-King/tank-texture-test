#pragma once
#include "Core.h"
#include "Entity.h"



coreDef(Factory)

std::vector<Entity> m_entities;

Entity *newEntity()
{
	for (int i = 0; i < m_entities.size(); ++i)
		if (!m_entities[i].active && !m_entities[i].newEntity)
		{
			m_entities[i].newEntity = true;
			return &m_entities[i];
		}

	m_entities.push_back(Entity(m_entities.size()));
	return &m_entities.back();
}

bool step()
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		////// new entities now allowed to update for the next frame
		if (m_entities[i].newEntity)
		{
			m_entities[i].active = true;
			m_entities[i].newEntity = false;			
		}
		////// old entities allowed to be removed for next frame
		if (m_entities[i].deleteEntity)
		{
			m_entities[i].active = false;
			m_entities[i].deleteEntity = false;
		}
	}
	return true;
}

bool init() 
{
	m_entities.reserve(64);
	return true; 
}
bool term() { return true; }

public:

	std::vector<Entity> &getEntities() { return m_entities; }

	void FreeEntity(Entity *e)
	{
		e->deleteEntity = true;
	}

	Entity *MakeTank(float x, float y, int faction, char up = 'W', char down = 'S', char left = 'A' , char right = 'D', char tLeft = 'Q', char tRight = 'E', char Fire = ' ')
	{
		Entity *tank = newEntity();
		Entity *turret = newEntity();

		compMake(tank, controller);
		compMake(tank, collider);
		compMake(tank, transform);
		compMake(tank, rigidbody);
		compMake(tank, render);
		compMake(tank, dependent);

		compMake(turret, controller);
		compMake(turret, transform);
		compMake(turret, render);
		compMake(turret, rigidbody);
		compMake(turret, collider);

		tank->dependent->child = turret;

		////// initialize position
		tank->transform->position().x = x;
		tank->transform->position().y = y;
		tank->transform->position().z = 0;
		////// init controllers
		tank->controller->forward = up;
		tank->controller->reverse = down;
		tank->controller->turn_Left = left;
		tank->controller->turn_Right = right;
		tank->controller->fire = 0;
		////// setup collisions
		tank->collider->faction = faction;
		//tank->collider->shape = MakeCircle(0, 0, 40);
		tank->collider->shape = MakeRect(0, 0, -30, -30, 30, 30);
		tank->controller->torque = 5;
		tank->controller->speed = 700;
		tank->rigidbody->mass = 1;
		tank->rigidbody->drag = 5;
		////// setup render context
		tank->render->shader = "Texture";

		////// setup ibo vbo vao and texture
		if (faction == 1)
		{
			tank->render->mesh = "Tank1";
			
			//printf("loading the green tank");
		}
		 
		else tank->render->mesh = "Tank1";
		tank->render->texture = "Tank_Base";
		tank->name = tank->render->mesh;		// For debugging
		

		////// Turret info
		turret->controller->speed = 0;
		turret->controller->torque = 10;
		turret->controller->turn_Left = tLeft;
		turret->controller->turn_Right = tRight;
		turret->controller->fire = Fire;
		turret->collider->faction = tank->collider->faction;
		turret->collider->shape.type = Shape::eNone;
		turret->rigidbody->drag = 10;
		turret->transform->m_parent = tank->transform;
		turret->render->shader = "Texture";
		turret->render->mesh = "Turret";
		turret->render->texture = "Tank_Turret";
		turret->name = turret->render->mesh;	// For debugging


		return tank;
	}


	Entity *MakeBullet(Transform *source, int faction, float speed)
	{
		Entity *bullet = newEntity();

		bullet->name = "Bullet";

		compMake(bullet, transform);
		compMake(bullet, render);
		compMake(bullet, collider);
		compMake(bullet, rigidbody);
		compMake(bullet, controller);
		compMake(bullet, lifeTime);

		bullet->transform->m_local = source->m_global;
		bullet->render->mesh    = "Bullet";
		bullet->render->texture = "BulletT";
		bullet->render->shader  = "Flat";
		bullet->collider->faction = faction;
		bullet->collider->shape = MakeCircle(0,0,7);
			//bullet->collider->shape = MakeRect(0, 0, -20, -10, 20, 40);
		bullet->rigidbody->velocity = bullet->transform->up() * speed;

		bullet->rigidbody->drag = 0;
		bullet->rigidbody->mass = 0.25f;
		bullet->lifeTime->totalLife = 5;

		bullet->controller->clear();

	    return bullet;
	}

	Entity *MakeAsteroid(float x, float y, int faction)
	{
		Entity *asteroid = newEntity();

		compMake(asteroid, collider);
		compMake(asteroid, transform);
		compMake(asteroid, rigidbody);
		compMake(asteroid, render);
		compMake(asteroid, dependent);

		asteroid->transform->position().x = x;
		asteroid->transform->position().y = y;

		asteroid->collider->faction = faction;

		asteroid->render->mesh = "Asteroid";
		asteroid->render->shader = "Flat";

		return asteroid;
	}


	Entity *CreatePlane(float x, float y, vec3 n, int faction)
	{
		Entity *plane = newEntity();

		compMake(plane, collider);
		compMake(plane, transform);

		plane->collider->faction = faction;
		plane->collider->shape = MakePlane(x, y, n);
		return plane;
	}
	
	Entity *CreateCircle(float x, float y, int r, int faction)
	{
		Entity *circle = newEntity();

		compMake(circle, collider);
		compMake(circle, transform);

		circle->collider->faction = faction;
		circle->collider->shape = MakeCircle(x, y, r);
		return circle;
	}

	Entity *CreateRect(float x, float y, int faction)
	{
		Entity *rect = newEntity();

		compMake(rect, collider);
		compMake(rect, transform);

		rect->collider->faction = faction;
		rect->transform->position() = vec3Make(x, y, 0);
		rect->collider->shape = MakeRect(0,0,-25,-25,25,25);

		return rect;
	}

	Entity *BGImage(int x, int y)
	{
		Entity *BG = newEntity();

		compMake(BG, transform);
		compMake(BG, render);

		BG->transform->position().x = x;
		BG->transform->position().y = y;
		BG->transform->position().z = 0;

		BG->render->mesh = "Anime";
		BG->render->shader = "Texture";
		BG->render->texture = "Finger";

		return BG;
	}

	Entity *Maketext(float x, float y, std::string text, int w = 16, int h = 24, Transform *parent = nullptr)
	{
		Entity *e = newEntity();
	
		compMake(e, transform);
		compMake(e, textRender);
	
		e->transform->m_local = makeScale(vec3Make(w, h, 1));
	
		e->textRender->text = text;
	
		e->transform->m_parent = parent;
		e->transform->position().x = x;
		e->transform->position().y = y;
		e->transform->position().z = 0;
	
		return e;
	}

	Entity *AddImage(float x, float y, float w, float h, std::string image)
	{
		Entity *e = newEntity();

		compMake(e, transform);
		compMake(e, render);

		e->render->mesh = "Quad";
		e->render->shader = "Texture";
		e->render->texture = image;
		
		e->transform->m_local = makeScale(vec3Make(w,h,1));

		e->transform->position() = vec3Make(x, y, 0);

		e->name = image;
		return e;

	}

coreEndDef