#pragma once

#include "Entity.h"

class Factory
{	
	ObjectPool<Entity>	  entities;
	
	// The factory will store an object pool for each component type
	ObjectPool<Transform> transforms;
	ObjectPool<Rigidbody> rigidbodies;
	ObjectPool<Collider>  colliders;
	ObjectPool<Sprite>    sprites;
	ObjectPool<Lifetime>  lifetimes;
	ObjectPool<Camera>    cameras;
	ObjectPool<Text>	  texts;
	ObjectPool<Health>	  healths;
	ObjectPool<PlayerController> controllers;
	ObjectPool<BossController> bosses;
	ObjectPool<CannonController> cannons;

public:

	// iterators to access the entity pool
	ObjectPool<Entity>::iterator begin() { return entities.begin(); }
	ObjectPool<Entity>::iterator end() { return entities.end(); }

	// for now, they're all the same size
	Factory(size_t size = 512)
								: entities(size), transforms(size), rigidbodies(size),
								  colliders(size), sprites(size), lifetimes(size),
								  cameras(size), controllers(size), texts(size), 
								  healths(size), bosses(size), cannons(size)
	{

	}

	// What follows are specialized spawning functions
	// just observe the steps taken and replicate for your own usages

	ObjectPool<Entity>::iterator spawnCamera(float w2, float h2, float zoom)
	{
		auto e = entities.push();
		e->transform = transforms.push();
		e->camera = cameras.push();
		e->camera->offset = vec2{w2,h2};
		e->camera->scale = vec2{ zoom,zoom };
		return e;
	}

	ObjectPool<Entity>::iterator spawnStaticImage(unsigned sprite, float x, float y, float w, float h)
	{
		auto e = entities.push();
		e->transform = transforms.push();
		e->sprite = sprites.push();
		e->sprite->sprite_id = sprite;
		e->sprite->dimensions = vec2{w,h};
		e->transform->setLocalPosition(vec2{ x,y });	
		return e;
	}

	ObjectPool<Entity>::iterator spawnBullet(unsigned sprite, vec2 pos, vec2 dim, float ang, float impulse, unsigned faction)
	{
		auto e = entities.push();
		e->type = BULLET;

		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->lifetime = lifetimes.push();
		e->collider = colliders.push();
		e->health = healths.push();

		e->transform->setLocalPosition(pos);
		e->transform->setLocalScale(dim);
		e->transform->setLocalAngle(ang);

		e->sprite->sprite_id = sprite;
		e->sprite->dimensions = vec2{1.2f, 1.2f};

		e->rigidbody->addImpulse(e->transform->getGlobalUp() * impulse);

		e->lifetime->lifespan = 1.7f;
		
		return e;
	}

	ObjectPool<Entity>::iterator spawnPlayer(unsigned sprite, unsigned font)
	{
		auto e = entities.push();
		e->type = PLAYER;
		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->collider = colliders.push();
		e->controller = controllers.push();
		e->text = texts.push();
		e->health = healths.push();
		e->health->health = 3;

		/*int health = e->health->health;
		char buffer[80];*/


		e->text->sprite_id = font;
		e->text->offset = vec2{ -24,-24 };
		e->text->off_scale = vec2{.4f,.4f};
		//sprintf_s(buffer ,80 ,"Health: %d", health);
		//e->text->setString(buffer);

		e->transform->setLocalScale(vec2{48,100});
		e->transform->setLocalAngle(-1.57);
		e->transform->setGlobalPosition(vec2{-350,0});

		e->sprite->sprite_id = sprite;

		return e;
	}


	ObjectPool<Entity>::iterator spawnAsteroid(unsigned sprite)
	{
		auto e = entities.push();
		e->type = ASTEROID;
		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->collider = colliders.push();
		e->health = healths.push();

		e->transform->setLocalScale(vec2{ 48,48 });

		e->transform->setGlobalPosition(vec2::fromAngle(randRange(0, 360)*DEG2RAD)*(rand01() * 200 + 64));

		e->rigidbody->addSpin(rand01()*12-6);

		e->sprite->sprite_id = sprite;

		return e;
	}

	ObjectPool<Entity>::iterator spawnCannon(vec2 offset, vec2 scale)
	{
		auto e = entities.push();
		e->transform = transforms.push();
		e->collider = colliders.push();
		e->health = healths.push();
		e->cannon = cannons.push();
		e->rigidbody = rigidbodies.push();

		e->transform->setLocalPosition(offset);
		e->transform->setLocalScale(scale);
		e->health->health = 4;

		return e;
	}

	ObjectPool<Entity>::iterator spawnBoss(unsigned sprite)
	{
		auto e = entities.push();


		e->type = BOSS;
		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->collider = colliders.push();
		e->health = healths.push();
		e->boss = bosses.push();

		// vec2{312,306};
		e->sprite->dimensions = vec2{1.56f,7.65f};


		e->health->health = 20;

		e->transform->setGlobalScale(vec2{ 200,40 });
		e->transform->setGlobalPosition(vec2{350,0});

		e->transform->addChild(
			&spawnCannon(vec2{ -.17f, 3.25f }, vec2{ 1,1 })->transform);
		e->transform->addChild(
			&spawnCannon(vec2{ .1f, 1.75f }, vec2{ 1,1 })->transform);
		e->transform->addChild(
			&spawnCannon(vec2{ .1f, -1.75f}, vec2{ 1,1 })->transform);
		e->transform->addChild(
			&spawnCannon(vec2{ -.17f, -3.25f }, vec2{ 1,1 })->transform);




		e->sprite->sprite_id = sprite;

		return e;
	}
	
	ObjectPool<Entity>::iterator spawnEnemyBullet(unsigned sprite, vec2 pos, vec2 dim, float ang, float impulse, unsigned faction)
	{
		auto e = entities.push();
		e->type = BULLET;

		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->lifetime = lifetimes.push();
		e->collider = colliders.push();
		e->health = healths.push();

		e->transform->setLocalPosition(pos);
		e->transform->setLocalScale(dim);
		e->transform->setLocalAngle(ang);

		e->sprite->sprite_id = sprite;
		e->sprite->dimensions = vec2{ 1.2f, 1.2f };

		e->rigidbody->addImpulse(vec2::fromAngle(ang) * impulse);

		e->lifetime->lifespan = 1.7f;

		return e;
	}

	ObjectPool<Entity>::iterator powerUp(unsigned line, vec2 pos, vec2 dim, float ang, float impulse, unsigned faction)
	{
		auto e = entities.push();
		e->type = POWERUP;

		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->lifetime = lifetimes.push();
		e->collider = colliders.push();
		e->health = healths.push();

		e->transform->setLocalPosition(pos);
		e->transform->setLocalScale(dim);
		e->transform->setLocalAngle(ang);

		
		e->sprite->dimensions = vec2{ 1.2f, 1.2f };

		e->rigidbody->addImpulse(vec2::fromAngle(ang) * impulse);

		e->lifetime->lifespan = 1.7f;

		return e;
	}

	ObjectPool<Entity>::iterator spawnLaser(unsigned sprite, vec2 pos, vec2 dim, float ang, float impulse, unsigned faction)
	{
		auto e = entities.push();
		e->type = BULLET;

		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->lifetime = lifetimes.push();
		e->collider = colliders.push();
		e->health = healths.push();

		e->transform->setLocalPosition(pos);
		e->transform->setLocalScale(dim);
		e->transform->setLocalAngle(ang);

		e->sprite->sprite_id = sprite;
		e->sprite->dimensions = vec2{ 1.2f, 1.2f };

		e->rigidbody->addImpulse(e->transform->getGlobalUp() * impulse);

		e->lifetime->lifespan = 1.7f;

		return e;
	}
};


