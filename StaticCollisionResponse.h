#pragma once

sysDef(StaticCollisionResponse)

bool init() { return true; }
bool term() { return true; }

bool condition(Entity *e)
	{	return e && e->active && e->collider;	}

bool specCondition(Entity *e, Entity *o)
{
	return e->rigidbody && !o->rigidbody || !e->rigidbody && o->rigidbody;
}

void update(Entity *e)
{
	
	for each(auto o in e->collider->contacts)
	{
		if (condition(o) && specCondition(e, o) && e->factoryIndex < o->factoryIndex)
		{
			auto &oCollider = o->collider->shape;
			auto &eCollider = e->collider->shape;
			vec3 N;

			if (oCollider.type == Shape::ePlane)
				N = oCollider.plane.normal;
			else
				N = eCollider.plane.normal;

			////// Circle Vs. Circle
			if (e->collider->shape.type == Shape::eCircle && o->collider->shape.type == Shape::eCircle)
			{
				N = normal( (eCollider.circle.position + e->transform->position()) - (oCollider.circle.position + o->transform->position()) );
			}
			////// Rectangle Vs. Rectangle
 			if (e->collider->shape.type == Shape::eRect && o->collider->shape.type == Shape::eRect)
			{
				e->collider->shape.orgin = e->transform->position();
				o->collider->shape.orgin = o->transform->position();
				N = AABBNormal(e->collider->shape.orgin + e->collider->shape.position + e->collider->shape.rect.min,
							   e->collider->shape.orgin + e->collider->shape.position + e->collider->shape.rect.max,
							   o->collider->shape.orgin + o->collider->shape.position + o->collider->shape.rect.min,
							   o->collider->shape.orgin + o->collider->shape.position + o->collider->shape.rect.max);
			}
			////// Rectangle Vs. Circle
			if (o->collider->shape.type == Shape::eRect && e->collider->shape.type == Shape::eCircle)
			{
				vec3 c = e->transform->position() + e->collider->shape.circle.position;		// -e->rigidbody->velocity;

				float minX = o->collider->shape.rect.min.x + o->transform->position().x;
				float maxX = o->collider->shape.rect.max.x + o->transform->position().x;
				float minY = o->collider->shape.rect.min.y + o->transform->position().y;
				float maxY = o->collider->shape.rect.max.y + o->transform->position().y;

				float cx = clamp(minX, maxX, c.x);
				float cy = clamp(minY, maxY, c.y);

				float scx = snap(minX, maxX, cx);
				float scy = snap(minY, maxY, cy);

				//if we're inside the circle
				if (abs(scx - cx) < abs(scy - cy))
					cx = scx;
				else cy = scy;

				if (cx == minX) N = vec3Make(-1, 0, 0);
				else if (cy == minY) N = vec3Make(0, 1, 0);
				else if (cx == maxX) N = vec3Make(1, 0, 0);
				else if (cy == maxY) N = vec3Make(0, -1, 0);
				
				vec3 corner = vec3Make(0, 0, 0);
				if (vec3Distance(vec3Make(minX, minY, 0), c) < eCollider.circle.radius)
					corner = vec3Make(minX, minY, 0);		//top right
				else if (vec3Distance(vec3Make(minX, maxY, 0), c) < eCollider.circle.radius)
					corner = vec3Make(minX, maxY, 0);		//bot right
				else if (vec3Distance(vec3Make(maxX, minY, 0), c) < eCollider.circle.radius)
					corner = vec3Make(maxX, minY, 0);		//top left
				else if (vec3Distance(vec3Make(maxX, maxY, 0), c) < eCollider.circle.radius)
					corner = vec3Make(maxX, maxY, 0);		//bot left

				if (corner.x != 0 || corner.y != 0)
				{
					N = normal(c - corner);
				}

			}
			if (o->collider->shape.type == Shape::eCircle && e->collider->shape.type == Shape::eRect)
			{
				float minX = oCollider.rect.min.x + o->transform->position().x;
				float minY = oCollider.rect.min.y + o->transform->position().y;

				float maxX = oCollider.rect.max.x + o->transform->position().x;
				float maxY = oCollider.rect.max.y + o->transform->position().y;

				vec3 cps = eCollider.circle.position + e->transform->position();

				float pX = clamp(minX, maxX, cps.x);
				float pY = clamp(minY, maxY, cps.y);

				float sX = snap(minX, maxX, pX);
				float sY = snap(minY, maxY, pY);

				if (abs(sX - pX) < abs(sY - pY))
					pX = sX; //is colliding in the x
				else
					pY = sY; //is colling in the y

				if (pX == minX)
				{	N = vec3Make(-1, 0, 0);	}
				else if (pX == maxX)
				{	N = vec3Make(1, 0, 0);	}
				else if (pY == minY)
				{	N = vec3Make(0, -1, 0);	}
				else if (pY == maxY)
				{	N = vec3Make(0, 1, 0);	}

				vec3 corner = vec3Make(0, 0, 0);
				if (vec3Distance(vec3Make(minX, minY, 0), cps) < eCollider.circle.radius)
					corner = vec3Make(minX, minY, 0);		//top right
				else if (vec3Distance(vec3Make(minX, maxY, 0), cps) < eCollider.circle.radius)
					corner = vec3Make(minX, maxY, 0);		//bot right
				else if (vec3Distance(vec3Make(maxX, minY, 0), cps) < eCollider.circle.radius)
					corner = vec3Make(maxX, minY, 0);		//top left
				else if (vec3Distance(vec3Make(maxX, maxY, 0), cps) < eCollider.circle.radius)
					corner = vec3Make(maxX, maxY, 0);		//bot left

				if (corner.x != 0 || corner.y != 0)
				{	N = normal(cps - corner);	}
			}


			if (e->rigidbody)
			{
				vec3 P = N * vec3Dot(e->rigidbody->velocity / -1, N);
				e->rigidbody->velocity = P * 2 + e->rigidbody->velocity;
			}
			else
			{
				vec3 P = N * vec3Dot(o->rigidbody->velocity / -1, N);
				o->rigidbody->velocity = P * 2 + o->rigidbody->velocity;
			}
		}
	}
}

sysEndDef