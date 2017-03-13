
#include "sfwdraw.h"
#include "GameState.h"
#include "MenuState.h"
#include "GameOverState.h"
#include "Level2State.h"
#include "FinalLevelState.h"
#include "Ending.h"
#include <cassert>


/*
	The main function should be used for application state management.
	Currently, only one state is implemented here, you'll need to add additional
	and get them to behave properly.
*/
void main()
{
	sfw::initContext();

	STATES curState = MENU_ENTER;

	GameState gs;
	Level2State l2s;
	FinalLevelState fs;
	MenuState ms;
	GameOverState gos;
	EndingState es;

	gs.init(); // called once
	l2s.init();
	fs.init();
	ms.init();
	es.init();
	gos.init();

	gs.play(); // Should be called each time the state is transitioned into

	while (sfw::stepContext())
	{
		switch (curState)
		{
		case MENU_ENTER:
			ms.play();
		case MENU:
			ms.draw();
			ms.step();
			ms.init();
			curState = (STATES)ms.next();
			break;
		case GAME_ENTER:
			gs.play();
		case GAME:
			gs.step();
			gs.draw();
			curState = (STATES)gs.next();
			break;
		case LEVEL2_ENTER:
			l2s.play();
		case LEVEL2:
			l2s.step();
			l2s.draw();
			curState = (STATES)l2s.next();
			break;
		case FINAL_LEVEL_ENTER:
			fs.play();
		case FINAL_LEVEL:
			fs.step();
			fs.draw();
			curState = (STATES)fs.next();
			break;
		case ENDING_ENTER:
			es.play();
		case ENDING:
			es.draw();
			es.step();
			es.init();
			curState = (STATES)es.next();
			break;
		case GAMEOVER_ENTER:
			gos.play();
		case GAMEOVER:
			gos.draw();
			gos.step();
			gos.init();
			curState = (STATES)gos.next();
			break;
		}

		//gs.next(); Determine the ID of the next state to transition to.
	}

	gs.stop(); // should be called each time the state is transitioned out of

	gs.term(); // called once


	sfw::termContext();

}