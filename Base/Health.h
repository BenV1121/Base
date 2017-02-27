#pragma once




class Health
{
public:
	int health = 1;

	bool isAlive()
	{
		if (health <= 0)
		{
			return false;
		}
		return true;
	}
};