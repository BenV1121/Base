#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"
#include "Health.h"


/*
The gamestate represents a discrete container of all that is
necessary to drive the game experience.

A factory object is used to manage the creation and deletion of
objects in the simulation.

The game state updates the entities within the factory using
a series of 'systems.'
*/

class FinalLevelState : public BaseState
{
	Factory factory;
	unsigned spr_space, spr_ship, spr_bullet, spr_boss, spr_font, spr_scroll, spr_ebullet;
	ObjectPool<Entity>::iterator currentCamera;
	bool isGameOver = false;
	bool win = false;
	//Health health;

public:
	virtual void init()
	{
		spr_bullet = sfw::loadTextureMap("../res/bullet2.png");
		spr_space = sfw::loadTextureMap("../res/BG3.png");
		spr_ship = sfw::loadTextureMap("../res/Ship2.png");
		//spr_roid = sfw::loadTextureMap("../res/rock.png");
		spr_boss = sfw::loadTextureMap("../res/boss.png");
		spr_ebullet = sfw::loadTextureMap("../res/enemybullet.png");
		spr_font = sfw::loadTextureMap("../res/font.png", 32, 4);
	}

	virtual void play()
	{
		isGameOver = false;
		win = false;
		// delete any old entities sitting around
		for (auto it = factory.begin(); it != factory.end(); it->onFree(), it.free());

		// setup a default camera
		currentCamera = factory.spawnCamera(800, 600, 1);
		currentCamera->transform->setGlobalPosition(vec2{ 400, 300 });

		// call some spawning functions!
		factory.spawnStaticImage(spr_space, 0, 0, 800, 600);

		factory.spawnPlayer(spr_ship, spr_font);
		factory.spawnBoss(spr_boss);
	}

	virtual void stop()
	{

	}

	// should return what state we're going to.
	// REMEMBER TO HAVE ENTRY AND STAY states for each application state!
	virtual size_t next() const
	{
		if (isGameOver)
			return GAMEOVER_ENTER;
		if (win)
			return ENDING_ENTER;
		else
			return 7;
	}

	// update loop, where 'systems' exist
	virtual void step()
	{
		float dt = sfw::getDeltaTime();
		int numBoss = 0;

		// maybe spawn some asteroids here.

		for (auto it = factory.begin(); it != factory.end();) // no++!
		{
			if (it->type == BOSS)
				numBoss++;

			bool del = false; // does this entity end up dying?
			auto &e = *it;    // convenience reference

							  // rigidbody update
			if (e.transform && e.rigidbody)
				e.rigidbody->integrate(&e.transform, dt);

			// controller update
			if (e.transform && e.rigidbody && e.controller)
			{
				e.controller->poll(&e.transform, &e.rigidbody, dt);
				if (e.controller->shotRequest) // controller requested a bullet fire
				{
					factory.spawnBullet(spr_bullet, e.transform->getGlobalPosition() + e.transform->getGlobalUp() * 60,
						vec2{ 32,32 }, e.transform->getGlobalAngle(), 400, 1);
				}
			}


			if (e.boss && e.rigidbody)
			{
				e.boss->update(dt, *e.rigidbody);
			}

			if (e.cannon)
			{
				e.cannon->update(dt);
				if (e.cannon->doFire)
				{
					factory.spawnEnemyBullet(spr_ebullet, e.transform->getGlobalPosition() + vec2{-132,0}, vec2{ 32,32 }, 180 * DEG2RAD, 400, 2);
				}
			}

			//factory.spawnEnemyBullet(spr_ebullet, e.transform->getGlobalPosition() + e.transform->getGlobalUp() * -60,
				//vec2{ 32,32 }, e.transform->getGlobalAngle(), 400, 1);

			// lifetime decay update
			if (e.lifetime)
			{
				e.lifetime->age(dt);
				if (!e.lifetime->isAlive())
					del = true;
			}

			// health update
			if (e.health)
			{
				if (!e.health->isAlive())
				{
					del = true;

					if (e.controller)
					{
						isGameOver = true;
					}
				}
			}

			// ++ here, because free increments in case of deletions
			if (!del) it++;
			else
			{
				it->onFree();
				it.free();
			}
		}

		if (numBoss == 0)
		{
			win = true;
		}

		// Physics system!
		// You'll want to extend this with custom collision responses


		for (auto it = factory.begin(); it != factory.end(); it++) // for each entity
			for (auto bit = it; bit != factory.end(); bit++)		  // for every other entity
				if (it != bit && it->transform && it->collider && bit->transform && bit->collider)
					// if they aren't the same and they both have collidable bits...
				{
					// test their bounding boxes
					if (base::BoundsTest(&it->transform, &it->collider, &bit->transform, &bit->collider))
					{
						// if true, get the penetration and normal from the convex hulls
						auto cd = base::ColliderTest(&it->transform, &it->collider, &bit->transform, &bit->collider);

						// if there was a collision,
						if (cd.result())
						{
							// condition for dynamic resolution
							if (it->rigidbody && bit->rigidbody)
							{
							//	base::DynamicResolution(cd, &it->transform, &it->rigidbody, &bit->transform, &bit->rigidbody);
								//std::cout << "-1";

								if (it->health)
									it->health->health -= 1;

								if (bit->health)
									bit->health->health -= 1;
							}

							// condition for static resolution
							else if (it->rigidbody && !bit->rigidbody)
								base::StaticResolution(cd, &it->transform, &it->rigidbody);
						}
					}
				}
	}


	virtual void draw()
	{
		// kind of round about, but this is the camera matrix from the factory's current camera
		auto cam = currentCamera->camera->getCameraMatrix(&currentCamera->transform);

		// draw sprites
		for each(auto &e in factory)
			if (e.transform && e.sprite)
				e.sprite->draw(&e.transform, cam);

		// draw text
		for each(auto &e in factory)
			if (e.transform && e.text)
				e.text->draw(&e.transform, cam);


#ifdef _DEBUG
		for each(auto &e in factory)
			if (e.transform)
				e.transform->draw(cam);

		for each(auto &e in factory)
			if (e.transform && e.collider)
				e.collider->draw(&e.transform, cam);

		for each(auto &e in factory)
			if (e.transform && e.rigidbody)
				e.rigidbody->draw(&e.transform, cam);
#endif
	}
};