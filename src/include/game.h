/*
 *  Game.h
 *  
 *  Created by LoGi on 03/07/2015
 *  Copyright 2015 All rights reserved
 */

#ifndef GAME_H
#define GAME_H

#include <stdlib.h>  	// For random numbers
#include <stdio.h> 		// Includes C
#include <time.h>       // For timers
#include <math.h>		// For mathematics
#include <iostream>		// For checking the keyboard characters
#include <nds.h> 		// Includes libnds
#include <fat.h>		// For filesystem
#include <nf_lib.h> 	// Includes NightFox Lib
#include <maxmod9.h>	// Audio library
#include "soundbank.h"
#include "soundbank_bin.h"

#include "spaceship.h"  // Player spaceship object
#include "bullet.h"  	// Bullet object
#include "alien.h"  	// Alien object
#include "barrier.h"  	// Barrier object
#include "ufo.h"  		// Ufo object

#define TIMER_SPEED (BUS_CLOCK/1024)

#define ALIEN_ROWS 3
#define ALIEN_COLUMNS 10

#define LARGE_ALIEN_ROWS 5
#define LARGE_ALIEN_COLUMNS 8

#define GIANT_ALIEN_ARRAY 3

#define SPECIAL_LEVEL 5
#define FINAL_LEVEL 9


using namespace std;

enum gameStateManger {GSM_MENU, GSM_SINGLEPLAYER, GSM_PAUSE, GSM_END};	
enum menuStateManger {MSM_MENU_ARCADE, MSM_MENU_CONTROLS, MSM_MENU_HIGHSCORE, MSM_MENU_QUIT};


class Game {

protected:

	int _argc;
	char **_argv;
	
	uint ticks;
	u8 screenTimer;
	u16 level;
	u16 alienNumber;
	bool gamePaused;
	bool ufoUsed;
	bool highScoreDisplay;
	
	gameStateManger gameState;
	menuStateManger menuState;
	
	Spaceship *playerShip;
	Bullet *playerBullet;
	
	Alien *alienArray[ALIEN_ROWS][ALIEN_COLUMNS];
	Bullet *alienBulletArray[ALIEN_ROWS][ALIEN_COLUMNS];
	
	Alien *largeAlienArray[LARGE_ALIEN_ROWS][LARGE_ALIEN_COLUMNS];
	Bullet *largeAlienBulletArray[LARGE_ALIEN_ROWS][LARGE_ALIEN_COLUMNS];
	
	Alien *giantAlienArray[GIANT_ALIEN_ARRAY];
	Bullet *giantAlienBulletArray[GIANT_ALIEN_ARRAY];
	
	Barrier *barrierArray[3];
	
	Ufo *ufo;
 
	mm_sound_effect alien_explode;
	mm_sound_effect player_hit;
	mm_sound_effect shoot_weapon;
	mm_sound_effect ufo_fly;
	mm_sound_effect level_complete;
	mm_sound_effect menu_beep;
 
	void LoadGameText();
	
	void UpdateGameText();
	
	void LoadSprites();
	
	void InitialiseMainMenu();
	
	void SetupGameMode();
	
	void InitialiseAudio();
	
	void ResetGame();
	
	void PlayGame();
	
	void HandleInput();
	
	void UpdateAliens();
	
	void MoveAliens();
	
	void AlienShoot();
	
	void UpdateAlienBullet();
	
	void CreateAliens(float speed);
	
	void CheckGameEnd();
	
	void LevelUpdate();

	void CreateUFO();

	void MovePlayer(float direction);
	
	void UpdatePlayerBullet();
	
	void DetectCollision();
	
	void PlayerShoot();

	void Initialise();
	
	void DisplayHighScore();
	
	void CheckHighScore(int score);
	
	void Update();
	
	void Nuke();
	
public:
     
    // Game constructor
    Game();

    // Game destructor
    virtual ~Game();
	 
	// Starts the game
	void StartGame(int argc, char **argv);
};

#endif
