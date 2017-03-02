#pragma once

#include "base\components\Rigidbody.h"

class BossController
{
public:

	float bandTimer = 0;
	float direction = 1;
	float range = 4;

	float speed = 60;


	void update(float dt, base::Rigidbody &rb)
	{
		bandTimer -= dt;

		rb.velocity = speed*direction*base::vec2{ 0,1 };

		if (bandTimer < 0)
		{
			direction *= -1;
			bandTimer = range;
		}
	}
};