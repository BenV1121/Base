#pragma once
#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"

using namespace sfw;

class GameOverState : public BaseState
{
	Factory factory;
	unsigned spr_logo;
	ObjectPool<Entity>::iterator currentCamera;

public:
	virtual void init()
	{
		spr_logo = loadTextureMap("../res/GameOver.png");
	}
	virtual void term() {}

	virtual void play()
	{
		factory.spawnStaticImage(spr_logo, 0, 0, 800, 600);

		currentCamera = factory.spawnCamera(800, 600, 1);
		currentCamera->transform->setGlobalPosition(vec2{ 400,300 });
	}
	virtual void stop() {}

	virtual size_t next() const
	{
		if (getKey(KEY_ENTER))
			return MENU_ENTER;
		else
			return GAMEOVER;
	}

	virtual void step() {}
	virtual void draw()
	{
		auto cam = currentCamera->camera->getCameraMatrix(&currentCamera->transform);

		// draw sprites
		for each(auto &e in factory)
			if (e.transform && e.sprite)
				e.sprite->draw(&e.transform, cam);
	}


};
