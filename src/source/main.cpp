/*
-------------------------------------------------
	Space Invaders NDS By Logi 2015
-------------------------------------------------
*/

#include <stdio.h> 		// Includes C
#include "game.h"  		// Game object


/************************************************* Main() ****************************************************************************************/
int main(int argc, char **argv) {

	Game *SpaceInvaders;
	SpaceInvaders = new Game;

	SpaceInvaders->StartGame(argc, argv);
	
	return 0;
}

