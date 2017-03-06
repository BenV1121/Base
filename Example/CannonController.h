#pragma once


class CannonController
{
public:
	float timer = 0;
	float delay = 1.2f;
	bool doFire = false;

	void update(float dt)
	{
		doFire = false;
		timer -= dt;
		if (timer < 0)
		{
			timer += delay;
			doFire = true;
		}
	}
};