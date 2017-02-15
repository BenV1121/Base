#pragma once

class BaseState
{

public:

	virtual void term() {}

	virtual void play() {}

	virtual void stop() {}

	virtual size_t next() const = 0;

	virtual void step() {}
	virtual void draw() {}
};

enum STATES
{
	MENU_ENTER,
	MENU,
	GAME_ENTER,
	GAME
};