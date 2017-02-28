#pragma once

#include "Base.h"

#include "PlayerController.h"
#include "Health.h"

using namespace base;

enum TYPE {GENERIC, ASTEROID, PLAYER, BULLET, BOSS};

class Entity
{
public:
	TYPE type = GENERIC;

	ObjectPool<Transform>::iterator transform;
	ObjectPool<Rigidbody>::iterator rigidbody;
	ObjectPool<Collider>::iterator  collider;
	ObjectPool<Lifetime>::iterator lifetime;
	ObjectPool<Sprite>::iterator sprite;
	ObjectPool<Camera>::iterator camera;
	ObjectPool<Text>::iterator text;

	// example of a component in this project
	ObjectPool<PlayerController>::iterator controller;
	ObjectPool<Health>::iterator health;

	void onFree()
	{
		transform.free();
		rigidbody.free();
		collider.free();
		lifetime.free();
		sprite.free();
		camera.free();
		text.free();
		health.free();

		controller.free();
	}
};