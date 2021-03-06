#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"
#include "Health.h"
//#include <vector>

/*
	The gamestate represents a discrete container of all that is 
	necessary to drive the game experience.

	A factory object is used to manage the creation and deletion of
	objects in the simulation.

	The game state updates the entities within the factory using
	a series of 'systems.'
*/

//class Spawnables
//{
//public:
//	std::vector<int> players;
//	std::vector<int> asteroids;
//	std::vector<int> enemies;
//	std::vector<int> backgrounds;
//};

class GameState : public BaseState
{
	Factory factory;
	unsigned spr_space, spr_ship, spr_bullet, spr_roid, spr_font, spr_scroll;
	ObjectPool<Entity>::iterator currentCamera;
	bool isGameOver = false;
	bool win = false;
	
	//Health health;
	
	
	/*std::vector<Spawnables> rounds;

	int curRound = 0;*/

public:
	virtual void init()
	{
		spr_bullet = sfw::loadTextureMap("../res/bullet2.png");
		spr_space = sfw::loadTextureMap("../res/BG.png");
		spr_ship = sfw::loadTextureMap("../res/Ship2.png");
		spr_roid = sfw::loadTextureMap("../res/rock.png");
		spr_font = sfw::loadTextureMap("../res/font.png",32,4);

		//Spawnables roundSpawns;
		////round one
		//roundSpawns.players.push_back(0);
		//roundSpawns.asteroids.push_back(0);
		//roundSpawns.asteroids.push_back(0);
		//roundSpawns.asteroids.push_back(0);
		//roundSpawns.asteroids.push_back(1);

		//rounds.push_back(roundSpawns);

		////round two
		//roundSpawns = Spawnables();
		//roundSpawns.players.push_back(0);
		//roundSpawns.asteroids.push_back(0);
		//roundSpawns.asteroids.push_back(1);
		//roundSpawns.asteroids.push_back(1);
		//roundSpawns.asteroids.push_back(1);

		//rounds.push_back(roundSpawns);
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
		factory.spawnAsteroid(spr_roid);
		factory.spawnAsteroid(spr_roid);
		factory.spawnAsteroid(spr_roid);
		factory.spawnAsteroid(spr_roid);

		//spawnRound();
	}

	//void spawnRound()
	//{
	//	for each(std::vector<int> player in rounds[curRound].players)
	//	{
	//		spawnPlayer(player);
	//	}
	//}

	virtual void stop()
	{

	}

	

	// should return what state we're going to.
	// REMEMBER TO HAVE ENTRY AND STAY states for each application state!
	virtual size_t next() const
	{ 
		if (isGameOver)
			return GAMEOVER_ENTER;
		else if (win)
			return LEVEL2_ENTER;
		else
			return 3;
	}

	// update loop, where 'systems' exist
	virtual void step()
	{
		float dt = sfw::getDeltaTime();
		int numAsteroids = 0;

		// maybe spawn some asteroids here.

		for(auto it = factory.begin(); it != factory.end();) // no++!
		{
			if (it->type == ASTEROID)
				numAsteroids++;

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
					factory.spawnBullet(spr_bullet, e.transform->getGlobalPosition()  + e.transform->getGlobalUp()*60,
											vec2{ 32,32 }, e.transform->getGlobalAngle(), 400, 1);
				}
			}
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
				if (e.controller)
				{
					int health = e.health->health;
					char buffer[80];

					sprintf_s(buffer, 80, "Health: %d", health);
					e.text->setString(buffer);
				}
				if (!e.health->isAlive())
				{
					del = true;
					
					if (e.controller)
					{
						isGameOver = true;
					}
				}
			}

			if (e.rigidbody)
			{
				if (e.controller)
				{

					// Boundaries: If the player moves out of the boundaries, it will spawn back.

					if (e.transform->getGlobalPosition().x <= -360)
					{
						e.rigidbody->velocity.x = -e.rigidbody->velocity.x;
						e.transform->setGlobalPosition(vec2{ -340,  e.transform->getGlobalPosition().y });
					}
					if (e.transform->getGlobalPosition().x >= 360)
					{
						e.rigidbody->velocity.x = -e.rigidbody->velocity.x;
						e.transform->setGlobalPosition(vec2{ 350,  e.transform->getGlobalPosition().y });
					}

					if (e.transform->getGlobalPosition().y <= -300)
					{
						e.rigidbody->velocity.y = -e.rigidbody->velocity.y;
						e.transform->setGlobalPosition(vec2{ e.transform->getGlobalPosition().x, -290 });
					}
					if (e.transform->getGlobalPosition().y >= 300)
					{
						e.rigidbody->velocity.y = -e.rigidbody->velocity.y;
						e.transform->setGlobalPosition(vec2{ e.transform->getGlobalPosition().x, 290 });
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

		if (numAsteroids == 0)
		{
			win = true;
		}

		// Physics system!
		// You'll want to extend this with custom collision responses

		
		for(auto it = factory.begin(); it != factory.end(); it++) // for each entity
			for(auto bit = it; bit != factory.end(); bit++)		  // for every other entity
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
								base::DynamicResolution(cd,&it->transform,&it->rigidbody, &bit->transform, &bit->rigidbody);
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