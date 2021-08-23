/*
 *  Game.cpp
 *  
 *  Created by LoGi on 05/07/2015
 *  Copyright 2015 All rights reserved
 */

#include "game.h"

// Game constructor
Game::Game() {

	gamePaused = false;
	ticks = 0;
	screenTimer = 0;
	ufoUsed = false;
	highScoreDisplay = false;
	
	level = 1;																// level counter
	alienNumber = 30;														// Alien counter
	gameState = GSM_MENU;													// Set the current game state to the main menu state
	menuState = MSM_MENU_ARCADE;											// Set the current menu state to the top menu item
	
	InitialiseAudio();
	
	playerShip = new Spaceship;         									// Create the players spaceship
	playerBullet = new Bullet(0, 0, 32, 0);									// Creates the player bullet
	
	ufo = new Ufo(0,0,36); // X, Y, ID
	
	// Create the array of aliens
	CreateAliens(0.28);// speed  //30
	
	// Create the array of barriers (3)
	u8 barrierX = 30;
	u8 spriteID = 33;
	
	for (int k = 0; k < 3; k++){
		barrierArray[k] = new Barrier(barrierX,162,spriteID); // X, Y , ID
		spriteID ++;
		barrierX += 82;
	}
}



// Game deconstructor
Game::~Game() {
	
	// Delete the barriers, giant aliens and giant alien bullets
	for (int i = 0; i < 3; i++){
		delete barrierArray[i];
		delete giantAlienArray[i];
		delete giantAlienBulletArray[i];
	}

	// Delete the standard aliens and the standard alien bullets
	for (int i = 0; i < ALIEN_ROWS; i++){
		for (int j = 0; j < ALIEN_COLUMNS; j++){
			delete alienArray[i][j];
			delete alienBulletArray[i][j];
		}
	}

	// Deletes the player and the player bullet
	delete playerShip;
	delete playerBullet;

	// Deletes the UFO
	delete ufo;
}



// Starts the game
void Game::StartGame(int argc, char **argv) {

	_argc = argc;
	_argv = argv;

	Initialise();
	InitialiseMainMenu();
	PlayGame();
}


// Initialise the game audio settings
void Game::InitialiseAudio() {

	// Initialise the maxmod soundbank
	mmInitDefaultMem((mm_addr)soundbank_bin);
	
	// Load the background audio files
	mmLoad( MOD_MENU_BACKGROUND );
	mmLoad( MOD_MAIN_BACKGROUND );
	
	// Load sound effect files
	mmLoadEffect(SFX_INVADER_KILLED);
	mmLoadEffect(SFX_PLAYER_HIT);
	mmLoadEffect(SFX_SHOOT);
	mmLoadEffect(SFX_UFO);
	mmLoadEffect(SFX_LEVEL_COMPLETE);

	// Sound effect settings
	//alien_explode = {{SFX_INVADER_KILLED}, (int)(1.0f * (1<<10)), 0, 50, 255,};  		// id, rate, handle, volume, panning
	//player_hit = {{SFX_PLAYER_HIT}, (int)(1.0f * (1<<10)), 0, 400, 255,};  			// id, rate, handle, volume, panning
	//shoot_weapon = {{SFX_SHOOT}, (int)(1.0f * (1<<10)), 0, 50, 255,};  				// id, rate, handle, volume, panning
	//ufo_fly = {{SFX_UFO}, (int)(1.0f * (1<<10)), 0, 1000, 255,};  					// id, rate, handle, volume, panning
	//level_complete = {{SFX_LEVEL_COMPLETE}, (int)(1.0f * (1<<10)), 0, 1000, 255,};  	// id, rate, handle, volume, panning
	//menu_beep = {{SFX_BEEP}, (int)(1.0f * (1<<10)), 0, 900, 255,};  					// id, rate, handle, volume, panning

	//mmSetModuleVolume(100); // Audio volume (0 - 1024)
}


void Game::CreateAliens(float speed) {
	
	if (level != SPECIAL_LEVEL && level != FINAL_LEVEL) {
	
		// Create the array of standard aliens and alien bullets (3 rows, 10 columns)
		float alienX;
		float alienY;
		u8 spriteID = 1;
		
		for (int i = 0; i < ALIEN_ROWS; i++){
			for (int j = 0; j < ALIEN_COLUMNS; j++){
				
				if (i == 0) {alienY = 5;} else if (i == 1) {alienY = 30;} else {alienY = 55;}
				if (j == 0) {alienX = 30;}
				
				srand(time(NULL));
				float shootPosition = rand() % 255 - 16;
				
				alienArray[i][j] = new Alien(alienX, alienY, spriteID, shootPosition, speed, 0); 			// X, Y , ID, shoot position, speed, size
				alienBulletArray[i][j] = new Bullet(alienX, alienY, spriteID + 50, 0); 						// X, Y , ID

				alienX += 20;
				spriteID ++;
			}
		}
	} else if (level == SPECIAL_LEVEL || level == FINAL_LEVEL) {
	
		// Create the array of giant aliens and alien bullets (1 row, 3 columns)
		srand(time(NULL));
		float shootPosition = rand() % 255 - 16;
		float _positionX = 15;
		u8 spriteID = 37;

		for (int i = 0; i < GIANT_ALIEN_ARRAY; i++){
			giantAlienArray[i] = new Alien(_positionX, 5, spriteID, shootPosition, speed, 2); 				// X, Y , ID, shoot position, speed, size
			giantAlienBulletArray[i] = new Bullet(_positionX, 5, spriteID + 3, 1); 							// X, Y , ID
			spriteID ++;
			_positionX += 80;
		}	
	}
}


// Sets up the game
void Game::SetupGameMode(){
		
	ufoUsed = false;	
	LoadSprites();																													// Loads the game sprite
	LoadGameText();																													// Initialise the text system on the top screen and displays the game text
	NF_UpdateTextLayers();									 																		// Update text layers
}


// Initialise the game
void Game::Initialise() {

	NF_Set2D(0, 0);																													// Set 2D MODE-0 for the Top screen
	NF_Set2D(1, 0);																													// Set 2D MODE-0 for the Bottom screen
	
	consoleDemoInit();
	iprintf("\n Initialising NitroFS.\n");
	iprintf("\n Pleaser wait a moment...\n\n\n\n\n");
	
	iprintf("\n If this message appears \n");
	iprintf("\n for longer than 5 seconds, \n");
	iprintf("\n your current loader does \n");
	iprintf("\n not support NitroFS \n\n");
	
	swiWaitForVBlank();
	
	NF_SetRootFolder("NITROFS");  																									// Set the Root Directory to NITRO FS																													// Load game menu screen
	
	menuState = MSM_MENU_ARCADE;																									// Set the current menu state
	gameState = GSM_MENU;                   																						// Set the current game state
}



// Reset Game
void Game::ResetGame() {

	for (int k = 0; k < 3; k++){barrierArray[k]->Reset();}
	
	float alienX;
	float alienY;

	for (int i = 0; i < ALIEN_ROWS; i++){
		for (int j = 0; j < ALIEN_COLUMNS; j++){
			
			if (i == 0) {alienY = 5;} else if (i == 1) {alienY = 30;} else {alienY = 55;}
			if (j == 0) {alienX = 30;}
			
			alienArray[i][j]->SetPosX(alienX);
			alienArray[i][j]->SetPosY(alienY);
			alienArray[i][j]->Reset();
			alienBulletArray[i][j]->Reset();
			alienX += 20;
		}
	}
	
	ufoUsed = false;
	level = 1;																
	alienNumber = 30;

	playerShip->Reset();	
	playerBullet->Reset();	
	
	NF_ResetTiledBgBuffers();
	NF_ResetSpriteBuffers();
}


// Load the game text
void Game::LoadGameText() {

	u8 length = 17;
				
	char playerLives[length];
	sprintf(playerLives, "%d", playerShip->GetPlayerLives());	
	
	char playerScore[length];
	sprintf(playerScore, "%d", playerShip->GetPlayerScore());
	
	NF_InitTextSys(0);	                                     																		// Initialize text system on top screen

	NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);	 																		// Load font
	NF_CreateTextLayer(0, 0, 0,	"normal");				     																		// Normal (layer 0)
	
	NF_WriteText(0, 0, 5, 22, playerLives);			 																				// Write text (layer 0)
	NF_WriteText(0, 0, 27, 22, playerScore);			 																			// Write text (layer 0)
}


// Update the game text
void Game::UpdateGameText() {

	u8 length = 17;

	char playerLives[length];
	sprintf(playerLives, "%d", playerShip->GetPlayerLives());	
	
	char playerScore[length];
	sprintf(playerScore, "%d", playerShip->GetPlayerScore());
	
	NF_ClearTextLayer(0, 0);  																										// Clear text layer 0 on the top screen
	
	if (playerShip->GetPlayerLives() >= 0) {NF_WriteText(0, 0, 5, 22, playerLives);}												// Write text (layer 0)
	if (playerShip->GetPlayerLives() >= 0) {NF_WriteText(0, 0, 27, 22, playerScore);}												// Write text (layer 0)
}	


// Loads the games main menu screen
void Game::InitialiseMainMenu(){
	
	NF_Set2D(0, 0);																													// Set 2D MODE-0 for the Top screen (To allow 16bit bitmap)
	NF_Set2D(1, 0);																													// Set 2D MODE-0 for the Bottom screen (To allow 16bit bitmap)
	
	NF_InitTiledBgBuffers();																										// Initialise the Background Buffers
	NF_InitTiledBgSys(0);																											// Initialise the Top Screen BgSystems
	NF_InitTiledBgSys(1);																											// Initialise the Bottom Screen BgSystems
	NF_LoadTiledBg("bg/Top_Menu_Screen", "TopSplash", 256, 256);																	// Load the top screen background image into RAM
	NF_LoadTiledBg("bg/Menu_1", "Menu1", 256, 256);																					// Load the top screen background image into RAM
	NF_LoadTiledBg("bg/Menu_2", "Menu2", 256, 256);																					// Load the top screen background image into RAM
	NF_LoadTiledBg("bg/Menu_3", "Menu3", 256, 256);																					// Load the top screen background image into RAM
	NF_LoadTiledBg("bg/Menu_4", "Menu4", 256, 256);																					// Load the top screen background image into RAM
	NF_LoadTiledBg("bg/Controls_Screen", "ControlScreen", 256, 256);																// Load the top screen background image into RAM
	NF_LoadTiledBg("bg/HighScores_Screen", "HighScoresScreen", 256, 256);															// Load the top screen background image into RAM
	
	NF_CreateTiledBg(0, 3, "TopSplash");																							// Create the top screen background (Highlight 1 player mode)
	NF_CreateTiledBg(1, 3, "Menu1");																								// Create the bottom screen background (Highlight 1 player mode)
}


// Loads the game sprites
void Game::LoadSprites() {

	NF_InitSpriteBuffers();																											// Initialise Sprite Buffers
	NF_InitSpriteSys(1);																											// Initialise Bottom Screen SpriteSystem

	// Creates and draws the player spaceship
	playerShip->Load();
	playerShip->Draw();
	
	// Creates the 3 standard alien sprites
	NF_LoadSpriteGfx("sprite/Alien_1_Small", 1, 16, 16);  																			// Loads the image for the standard alien 1 sprite										
	NF_LoadSpriteGfx("sprite/Alien_2_Small", 2, 16, 16);  																			// Loads the image for the standard alien 2 sprite	
	NF_LoadSpriteGfx("sprite/Alien_3_Small", 3, 16, 16);  																			// Loads the image for the standard alien 3 sprite									
	
	// Creates the 3 giant alien sprites
	NF_LoadSpriteGfx("sprite/Alien_1_Large", 101, 64, 64);  																		// Loads the image for the giant alien 1 sprite										
	NF_LoadSpriteGfx("sprite/Alien_2_Large", 102, 64, 64);  																		// Loads the image for the giant alien 2 sprite	
	NF_LoadSpriteGfx("sprite/Alien_3_Large", 103, 64, 64);  																		// Loads the image for the giant alien 3 sprite
	
	NF_LoadSpritePal("sprite/Alien", 1);																							// Loads the shared colour pallete for the alien sprites		
	NF_VramSpritePal(1, 1, 1);				//(Screen, RAM Slot, VRAM Slot)					  										// Loads the alien sprite colour Palette into VRAM
	
	NF_VramSpriteGfx(1, 1, 1, false);	//(Screen, RAM Slot, VRAM Slot, keepframes)													// Loads the standard alien 1 sprite into VRAM
	NF_VramSpriteGfx(1, 2, 2, false);	//(Screen, RAM Slot, VRAM Slot, keepframes)													// Loads the standard alien 2 sprite into VRAM
	NF_VramSpriteGfx(1, 3, 3, false);	//(Screen, RAM Slot, VRAM Slot, keepframes)													// Loads the standard alien 3 sprite into VRAM
	
	NF_VramSpriteGfx(1, 101, 101, false);	//(Screen, RAM Slot, VRAM Slot, keepframes)												// Loads the giant alien 1 sprite into VRAM
	NF_VramSpriteGfx(1, 102, 102, false);	//(Screen, RAM Slot, VRAM Slot, keepframes)												// Loads the giant alien 2 sprite into VRAM
	NF_VramSpriteGfx(1, 103, 103, false);	//(Screen, RAM Slot, VRAM Slot, keepframes)												// Loads the giant alien 3 sprite into VRAM
	
	if (level != SPECIAL_LEVEL && level != FINAL_LEVEL) {
	
		// Draws the standard alien sprites
		u8 alienType = 1;
		for (int i = 0; i < ALIEN_ROWS; i++){
			for (int j = 0; j < ALIEN_COLUMNS; j++){
				if (i == 0) {
					alienType = 1;
					alienArray[i][j]->SetPoints(20);
				} else if (i == 1) {
					alienType = 2;
					alienArray[i][j]->SetPoints(10);
				} else {
					alienType = 3;
					alienArray[i][j]->SetPoints(5);
				}
				
				alienArray[i][j]->Draw(alienType);
			}
		}
	} else if (level == SPECIAL_LEVEL || level == FINAL_LEVEL) {
	
		// Draws the giant alien sprites
		for (int i = 0; i < GIANT_ALIEN_ARRAY; i++){
			giantAlienArray[i]->SetPoints(5);
			giantAlienArray[i]->Draw(i + 101);
		}
		
		giantAlienBulletArray[0]->Load();
	} 
		
	// Loads the bullet sprite
	playerBullet->Load();
	
	// Creates the barrier sprite
	barrierArray[0]->Load();
	
	// Draws the 3 barrier sprites
	for (int i = 0; i < 3; i++){barrierArray[i]->Draw();}

	ufo->Load();

	// Pause menu sprite
	NF_LoadSpriteGfx("sprite/Pause", 105, 64, 32);  																				// Loads the image for the giant alien 3 sprite
	NF_VramSpriteGfx(1, 105, 105, false);	//(Screen, RAM Slot, VRAM Slot, keepframes)												// Loads the giant alien 3 sprite into VRAM
}


// Update the game (Not used yet!)
void Game::Update() {

	//UpdateGameText();
}


// Move the alien sprites
void  Game::MoveAliens(){

	if (level != SPECIAL_LEVEL && level != FINAL_LEVEL) {

		// This moves all of the standard aliens left and right, incrementing the y position when a border is reached
		for (int i = 0; i < ALIEN_ROWS; i++){
			for (int j = 0; j < ALIEN_COLUMNS; j++){
			
				// Updates the aliens
				alienArray[i][j]->Update();
			
				// If the aliens are currently moving left
				if ((alienArray[i][j]->GetDirection() == 0) && (alienArray[i][j]->GetPosX() <= 0) && alienArray[i][j]->IsAlive()){                     		
				
					// Loop through the entire alien array, incrementing the Y position and changing the direction
					for (int k = 0; k < ALIEN_ROWS; k++){
						for (int l = 0; l < ALIEN_COLUMNS; l++){
							alienArray[k][l]->SetPosY(alienArray[k][l]->GetPosY() +10);
							alienArray[k][l]->SetDirection(1);
							alienArray[k][l]->SetShootPos(rand() % 255 - 16);
						}		
					}
					
					for (int k = 0; k < 3; k++){alienArray[k][0]->SetPosX(alienArray[k][1]->GetPosX() -20);} // Fixes the wobble effect
					
				} else if ((alienArray[i][j]->GetDirection() == 1) && (alienArray[i][j]->GetPosX() >= SCREEN_WIDTH - 16) && alienArray[i][j]->IsAlive()){		
																			
					// Loop through the entire alien array, incrementing the Y position and changing the direction
					for (int k = 0; k < ALIEN_ROWS; k++){
						for (int l = 0; l < ALIEN_COLUMNS; l++){
							alienArray[k][l]->SetPosY(alienArray[k][l]->GetPosY() +10);
							alienArray[k][l]->SetDirection(0);
							alienArray[k][l]->SetShootPos(rand() % 255 - 16);	
						}		
					}
					
					for (int k = 0; k < 3; k++){alienArray[k][0]->SetPosX(alienArray[k][1]->GetPosX() -20);} // Fixes the wobble effect
				} 
			}		
		}
	
	
	} else if (level == SPECIAL_LEVEL || level == FINAL_LEVEL) {
	
		// This moves all of the giant aliens left and right, incrementing the y position when a border is reached
		for (int i = 0; i < GIANT_ALIEN_ARRAY; i++){
		
			// Updates the giant alien sprites
			giantAlienArray[i]->Update();
	
			if ((giantAlienArray[i]->GetDirection() == 0) && (giantAlienArray[i]->GetPosX() <= 0) && giantAlienArray[i]->IsAlive()){
			
				// Loop through the entire alien array, incrementing the Y position and changing the direction
				for (int j = 0; j < GIANT_ALIEN_ARRAY; j++){
					
					float shootPosition;
					if (j == 0) { shootPosition = rand() % 85;}
					else if (j == 1) { shootPosition = rand() % 85 + 85;}
					else if (j == 2) { shootPosition = rand() % 180 + 10;}
					
					giantAlienArray[j]->SetShootPos(shootPosition);
					giantAlienArray[j]->SetDirection(1);
					giantAlienArray[j]->SetPosY(giantAlienArray[j]->GetPosY() +10);
				}

			} else if ((giantAlienArray[i]->GetDirection() == 1) && (giantAlienArray[i]->GetPosX() >= SCREEN_WIDTH - 64) && giantAlienArray[i]->IsAlive()){
			
				// Loop through the entire alien array, incrementing the Y position and changing the direction
				for (int k = 0; k < GIANT_ALIEN_ARRAY; k++){
				
					float shootPosition;
					if (k == 0) { shootPosition = rand() % 85;}
					else if (k == 1) { shootPosition = rand() % 85 + 85;}
					else if (k == 2) { shootPosition = rand() % 180 + 10;}
					
					giantAlienArray[k]->SetShootPos(shootPosition);
					giantAlienArray[k]->SetDirection(0);
					giantAlienArray[k]->SetPosY(giantAlienArray[k]->GetPosY() +10);	
				}
			}
		}
	} 
}



// Alien shoot bullet function
void  Game::AlienShoot(){
	
	if (level != SPECIAL_LEVEL && level != FINAL_LEVEL) {
	
		for (int i = 0; i < ALIEN_ROWS; i++){
			for (int j = 0; j < ALIEN_COLUMNS; j++){
			
				float shootChance = rand() % 3; //3
				float alienPos = ceil (alienArray[i][j]->GetPosX()); // Round the aliens current x position
				
				if (alienArray[i][j]->GetShootPosition() == alienPos && shootChance == 0 && alienArray[i][j]->IsAlive()) {
			
					if (alienBulletArray[i][j]->IsAlive() != true){
						alienBulletArray[i][j]->SetAlive(true);
						alienBulletArray[i][j]->SetPosX(alienArray[i][j]->GetPosX() + 4);
						alienBulletArray[i][j]->SetPosY(alienArray[i][j]->GetPosY() + 4);

						// Draws the aliens bullet sprite
						alienBulletArray[i][j]->Draw();
						
						// Play the shoot sound effect
						mmEffectEx(&shoot_weapon);
					}
				}
			}
		}
	} else if (level == SPECIAL_LEVEL || level == FINAL_LEVEL) {
	
		for (int i = 0; i < GIANT_ALIEN_ARRAY; i++){

			float alienPos = ceil (giantAlienArray[i]->GetPosX()); // Round the aliens current x position
			
			if (giantAlienArray[i]->GetShootPosition() == alienPos  && giantAlienArray[i]->IsAlive()) {
		
				if (giantAlienBulletArray[i]->IsAlive() != true){
					giantAlienBulletArray[i]->SetAlive(true);
					giantAlienBulletArray[i]->SetPosX(giantAlienArray[i]->GetPosX() + 24);
					giantAlienBulletArray[i]->SetPosY(giantAlienArray[i]->GetPosY() + 10);

					// Draws the giant aliens bullet sprite
					giantAlienBulletArray[i]->Draw();
					
					// Play the shoot sound effect
					mmEffectEx(&shoot_weapon);
				}
			}
		}
	} 
}




// Updates the alien bullet sprites
void  Game::UpdateAlienBullet(){

	// Updates the standard alien bullets
	if (level != SPECIAL_LEVEL && level != FINAL_LEVEL) {
		for (int i = 0; i < ALIEN_ROWS; i++){
			for (int j = 0; j < ALIEN_COLUMNS; j++){
				if (alienBulletArray[i][j]->IsAlive()) {alienBulletArray[i][j]->MoveDown();}
			}
		}
	} else if (level == SPECIAL_LEVEL || level == FINAL_LEVEL) {
	
		// Updates the giant alien bullets
		for (int i = 0; i < GIANT_ALIEN_ARRAY; i++){if (giantAlienBulletArray[i]->IsAlive()) {giantAlienBulletArray[i]->MoveDown();}}
	}
}


// Updates the alien sprites
void  Game::UpdateAliens(){

	//AnimateAliens();
	MoveAliens();
	
	AlienShoot();
	UpdateAlienBullet();
	
	// Creates the ufo's on each level
	if (level == 1 && alienNumber == 21) {CreateUFO();}
	else if (level == 2 && alienNumber == 17) {CreateUFO();}
	else if (level == 3 && alienNumber == 11) {CreateUFO();}
	else if (level == 4 && alienNumber == 7) {CreateUFO();}
	else if (level == 5 && alienNumber == 9) {CreateUFO();}
	else if (level == 6 && alienNumber == 5) {CreateUFO();}
	else if (level == 7 && alienNumber == 16) {CreateUFO();}
	else if (level == 8 && alienNumber == 12) {CreateUFO();}
	else if (level == 9 && alienNumber == 9) {CreateUFO();}
}



// Updates the bullet sprite
void  Game::UpdatePlayerBullet(){

	if (playerBullet->IsAlive()) {playerBullet->MoveUp();}
}


// Fires a bullet from the player spaceship
void Game::PlayerShoot(){
	
	// Prevents the player from shooting if they are behind a barrier
	u8 width = 8;
	
	if ((playerShip->GetPosX() +width >= barrierArray[0]->GetPosX() && playerShip->GetPosX() +width <= (barrierArray[0]->GetPosX() +32) && barrierArray[0]->IsAlive()) ||
		(playerShip->GetPosX() +width >= barrierArray[1]->GetPosX() && playerShip->GetPosX() +width <= (barrierArray[1]->GetPosX() +32) && barrierArray[1]->IsAlive()) ||
		(playerShip->GetPosX() +width >= barrierArray[2]->GetPosX() && playerShip->GetPosX() +width <= (barrierArray[2]->GetPosX() +32) && barrierArray[2]->IsAlive())) 
	{
		// Do nothing
	} else {
	
		// Prevents the player from shooting multiple bullets
		if (playerBullet->IsAlive() != true && playerShip->GetFrame() != 1) {
		
			// Create the bullet in front of the players spaceship sprite
			playerBullet = new Bullet((playerShip->GetPosX() + 6), (playerShip->GetPosY()), 32, 0);
			playerBullet->SetAlive(true);
			
			// Draw the players bullet sprite
			playerBullet->Draw();	

			// Play the shoot sound effect
			mmEffectEx(&shoot_weapon);
		}
	}	
}


// Moves the spaceship sprite
void Game::MovePlayer(float direction) {

	if (direction == 0) {playerShip->MoveLeft();} else {playerShip->MoveRight();}
}


// Level display
void Game::LevelUpdate() {

	ticks = 0;
	timerStart(0, ClockDivider_1024, 0, NULL);
	
	NF_LoadTiledBg("bg/Level1_Screen", "Level1Screen", 256, 256);	  							// Load a Background into RAM for Level 1 screen
	NF_LoadTiledBg("bg/Level2_Screen", "Level2Screen", 256, 256);	  							// Load a Background into RAM for Level 2 screen
	NF_LoadTiledBg("bg/Level3_Screen", "Level3Screen", 256, 256);	  							// Load a Background into RAM for Level 3 screen
	NF_LoadTiledBg("bg/Level4_Screen", "Level4Screen", 256, 256);	  							// Load a Background into RAM for Level 4 screen
	NF_LoadTiledBg("bg/Level5_Screen", "Level5Screen", 256, 256);	  							// Load a Background into RAM for Level 5 screen
	NF_LoadTiledBg("bg/Level6_Screen", "Level6Screen", 256, 256);	  							// Load a Background into RAM for Level 6 screen
	NF_LoadTiledBg("bg/Level7_Screen", "Level7Screen", 256, 256);	  							// Load a Background into RAM for Level 7 screen
	NF_LoadTiledBg("bg/Level8_Screen", "Level8Screen", 256, 256);	  							// Load a Background into RAM for Level 8 screen
	NF_LoadTiledBg("bg/Final_Level_Screen", "FinalLevelScreen", 256, 256);	  					// Load a Background into RAM for final level screen
	NF_LoadTiledBg("bg/GameOver_Screen", "GameOverScreen", 256, 256);	  						// Load a Background into RAM for game over screen
	NF_LoadTiledBg("bg/GameComplete_Screen", "GameCompleteScreen", 256, 256);	  				// Load a Background into RAM for game complete screen
	
	NF_CreateTiledBg(1, 3, "Level1Screen");						  								// Create the Bottom Background
	
	while(ticks < 60000){ticks += timerElapsed(0);}   // Add delay
}



void Game::CheckGameEnd() {

	ticks = 0;
	timerStart(0, ClockDivider_1024, 0, NULL);

	if (playerShip->GetPlayerLives() < 0){

		screenTimer ++;
		
		if (screenTimer == 70){
		
			NF_InitSpriteSys(1);
			NF_CreateTiledBg(1, 3, "GameOverScreen");
			gameState = GSM_END;
			
			while(ticks < 140000){ticks += timerElapsed(0);}  										// Add delay
			CheckHighScore(playerShip->GetPlayerScore());
		}
		
	} else if(alienNumber == 0 && level <=8) {

		screenTimer ++;
		
		if (screenTimer == 120){
			screenTimer = 0;
			
			if (level == 5) {playerShip->SetPlayerLives(playerShip->GetPlayerLives() + 1);}		// Increment the player lives by 1
			
			if (level == 1) {NF_CreateTiledBg(1, 3, "Level2Screen");}
			else if (level == 2) {NF_CreateTiledBg(1, 3, "Level3Screen");}
			else if (level == 3) {NF_CreateTiledBg(1, 3, "Level4Screen");}	
			else if (level == 4) {NF_CreateTiledBg(1, 3, "Level5Screen");}
			else if (level == 5) {NF_CreateTiledBg(1, 3, "Level6Screen");}
			else if (level == 6) {NF_CreateTiledBg(1, 3, "Level7Screen");}
			else if (level == 7) {NF_CreateTiledBg(1, 3, "Level8Screen");}
			else if (level == 8) {NF_CreateTiledBg(1, 3, "FinalLevelScreen");}
			
			mmEffectEx(&level_complete);   															// Play level complete sound effect
			while(ticks < 140000){ticks += timerElapsed(0);}  										// Add delay
				
			NF_CreateTiledBg(1, 3, "BottomGameScreen");	
			level ++;
	
			if (level != SPECIAL_LEVEL && level != FINAL_LEVEL) {alienNumber = 30;} else {alienNumber = 3;}
			
			// Each level the aliens speed increases except for the giant alien levels
			if (level == 2) {CreateAliens(0.40);}        
			else if (level == 3) {CreateAliens(0.45);}     
			else if (level == 4) {CreateAliens(0.50);}    
			else if (level == 5) {CreateAliens(0.35);}     // Giant alien level
			else if (level == 6) {CreateAliens(0.55);}    
			else if (level == 7) {CreateAliens(0.60);}     
			else if (level == 8) {CreateAliens(0.70);}     
			else if (level == 9) {CreateAliens(0.35);}     // Giant alien level     

			SetupGameMode();
		}
		
	} else if(alienNumber == 0 && level ==9) {
	
		screenTimer ++;
		
		if (screenTimer == 70){
		
			NF_InitSpriteSys(1);
			NF_CreateTiledBg(1, 3, "GameCompleteScreen");
			gameState = GSM_END;
			
			while(ticks < 140000){ticks += timerElapsed(0);}  // Add delay
			CheckHighScore(playerShip->GetPlayerScore());
		}
	}
}


void Game::CreateUFO() {

	if (!ufoUsed) {
	
		srand(time(NULL));	
		float ufoDirection = rand() % 2; // Random direction for the ufo

		if (ufoDirection == 0) {ufo->SetPosX(260);} else if (ufoDirection == 1){ufo->SetPosX(-20);}

		ufo->SetPosY(5);
		ufo->SetDirection(ufoDirection);
		ufo->Draw();
		ufoUsed = true;
		mmEffectEx(&ufo_fly);   // Play the ufo sound effect
	}
}



// Detect collisions between the bullets, aliens, barriers and the player
void  Game::DetectCollision(){

	// Collisions for the standard levels
	if (level != SPECIAL_LEVEL && level != FINAL_LEVEL) {
	
		for (int i = 0; i < ALIEN_ROWS; i++){
			for (int j = 0; j < ALIEN_COLUMNS; j++){
				// Detect collisions between the players bullet and the standard aliens
				if (alienArray[i][j]->IsAlive() && playerBullet->IsAlive()) {
				
					if (playerBullet->GetPosX() >= alienArray[i][j]->GetPosX() && playerBullet->GetPosX() <= (alienArray[i][j]->GetPosX() +16)
					&& playerBullet->GetPosY() >= alienArray[i][j]->GetPosY() && playerBullet->GetPosY() <= (alienArray[i][j]->GetPosY() +16)) {
						alienArray[i][j]->Hit();
						playerBullet->Hit();
						playerShip->SetPlayerScore(playerShip->GetPlayerScore() + alienArray[i][j]->GetPoints());
						alienNumber -= 1;
						mmEffectEx(&alien_explode);   // Play the alien explode sound effect
					}	
				}	
				// Detect collisions between the standard aliens bullet and the barrier sprites
				for (int k = 0; k < 3; k++){
				
					if (alienBulletArray[i][j]->IsAlive() && barrierArray[k]->IsAlive()) {
				
						if (alienBulletArray[i][j]->GetPosX() >= barrierArray[k]->GetPosX() && alienBulletArray[i][j]->GetPosX() <= (barrierArray[k]->GetPosX() +32)
						&& alienBulletArray[i][j]->GetPosY() >= barrierArray[k]->GetPosY() && alienBulletArray[i][j]->GetPosY() <= (barrierArray[k]->GetPosY() +32)) {
							alienBulletArray[i][j]->Hit();
							barrierArray[k]->Hit();
						}
					}
				}
				// Detect collisions between the standard aliens bullet and the player spaceship
				if (alienBulletArray[i][j]->IsAlive()) {
				
					if (alienBulletArray[i][j]->GetPosX() >= playerShip->GetPosX() && alienBulletArray[i][j]->GetPosX() <= (playerShip->GetPosX() +16)
					&& alienBulletArray[i][j]->GetPosY() >= playerShip->GetPosY() && alienBulletArray[i][j]->GetPosY() <= (playerShip->GetPosY() +16)) {
						alienBulletArray[i][j]->Hit();
						playerShip->Hit();
						mmEffectEx(&player_hit);   // Play the player hit sound effect
					}
				}
				// Detect collisions between the standard aliens and the barrier sprites
				for (int k = 0; k < 3; k++){
				
					if (alienArray[i][j]->IsAlive() && barrierArray[k]->IsAlive()) {
					
						if ((alienArray[i][j]->GetPosX() >= barrierArray[k]->GetPosX() && alienArray[i][j]->GetPosX() <= (barrierArray[k]->GetPosX() +32) 
						&& alienArray[i][j]->GetPosY() +16 >= barrierArray[k]->GetPosY() && alienArray[i][j]->GetPosY() +16 <= (barrierArray[k]->GetPosY() +32))
						|| (alienArray[i][j]->GetPosX() +16 >= barrierArray[k]->GetPosX() && alienArray[i][j]->GetPosX() +16 <= (barrierArray[k]->GetPosX() +32) 
						&& alienArray[i][j]->GetPosY() +16 >= barrierArray[k]->GetPosY() && alienArray[i][j]->GetPosY() +16 <= (barrierArray[k]->GetPosY() +32))) {
							alienArray[i][j]->Hit();
							alienNumber -= 1;
							playerShip->Hit();
							barrierArray[k]->Hit();
							mmEffectEx(&alien_explode);   // Play the alien explode sound effect
						}
					}
				}
				// Detect collisions between the standard aliens and the players spaceship sprite
				if (alienArray[i][j]->IsAlive()) {
				
					if ((alienArray[i][j]->GetPosX() >= playerShip->GetPosX() && alienArray[i][j]->GetPosX() <= (playerShip->GetPosX() +16)
					&& alienArray[i][j]->GetPosY() +16 >= playerShip->GetPosY() && alienArray[i][j]->GetPosY() +16 <= (playerShip->GetPosY() +16))
					|| (alienArray[i][j]->GetPosX() +16 >= playerShip->GetPosX() && alienArray[i][j]->GetPosX() +16 <= (playerShip->GetPosX() +16) 
					&& alienArray[i][j]->GetPosY() +16 >= playerShip->GetPosY() && alienArray[i][j]->GetPosY() +16 <= (playerShip->GetPosY() +16))) {
						alienArray[i][j]->Hit();
						alienNumber -= 1;
						playerShip->Hit();
						mmEffectEx(&player_hit);   // Play the player hit sound effect
					}
				}
				// Detect collisions between the players bullet and the ufo
				if (ufo->IsAlive() && playerBullet->IsAlive()) {
					
					if (playerBullet->GetPosX() >= ufo->GetPosX() && playerBullet->GetPosX() <= (ufo->GetPosX() +16)
					&& playerBullet->GetPosY() >= ufo->GetPosY() && playerBullet->GetPosY() <= (ufo->GetPosY() +14)) {
						
						ufo->Hit();
						playerBullet->Hit();
						playerShip->SetPlayerScore(playerShip->GetPlayerScore() + 100);
						mmEffectEx(&player_hit);   // Play the player hit sound effect
					}
				}
				// Detect if any standard aliens reach the upper bounds of the screen (bottom of screen in game)
				if (alienArray[i][j]->IsAlive()) {
				
					if (alienArray[i][j]->GetPosY() >= SCREEN_HEIGHT - 8) {
						alienArray[i][j]->Hit();
						alienNumber -= 1;
						playerShip->Hit();
						mmEffectEx(&player_hit);   // Play the player hit sound effect
					}	
				}	
			}
		}
	} else if (level == SPECIAL_LEVEL || level == FINAL_LEVEL) {
	
		// Collisions for the special levels
		for (int i = 0; i < GIANT_ALIEN_ARRAY; i++){
		
			// Detect collisions between the players bullet and the giant aliens
			if (giantAlienArray[i]->IsAlive() && playerBullet->IsAlive()) {
			
				if (playerBullet->GetPosX() >= giantAlienArray[i]->GetPosX() && playerBullet->GetPosX() <= (giantAlienArray[i]->GetPosX() +64)
				&& playerBullet->GetPosY() >= giantAlienArray[i]->GetPosY() && playerBullet->GetPosY() <= (giantAlienArray[i]->GetPosY() +64)) {
					
					giantAlienArray[i]->Hit();
					playerBullet->Hit();
					playerShip->SetPlayerScore(playerShip->GetPlayerScore() + giantAlienArray[i]->GetPoints());
					
					if (!giantAlienArray[i]->IsAlive()) {alienNumber -= 1;}
					
					mmEffectEx(&alien_explode);   // Play the alien explode sound effect
				}	
			}	
			// Detect collisions between the giant aliens bullet and the barrier sprites
			for (int k = 0; k < GIANT_ALIEN_ARRAY; k++){
			
				if (giantAlienBulletArray[i]->IsAlive() && barrierArray[k]->IsAlive()) {
			
					if (giantAlienBulletArray[i]->GetPosX() >= barrierArray[k]->GetPosX() && giantAlienBulletArray[i]->GetPosX() <= (barrierArray[k]->GetPosX() +32)
					&& giantAlienBulletArray[i]->GetPosY() >= barrierArray[k]->GetPosY() && giantAlienBulletArray[i]->GetPosY() <= (barrierArray[k]->GetPosY() +32)) {
					
						giantAlienBulletArray[i]->Hit();
						barrierArray[k]->Hit();
					}
				}
			}
			// Detect collisions between the giant aliens bullet and the player spaceship
			if (giantAlienBulletArray[i]->IsAlive()) {
			
				if (giantAlienBulletArray[i]->GetPosX() >= playerShip->GetPosX() && giantAlienBulletArray[i]->GetPosX() <= (playerShip->GetPosX() +16)
				&& giantAlienBulletArray[i]->GetPosY() >= playerShip->GetPosY() && giantAlienBulletArray[i]->GetPosY() <= (playerShip->GetPosY() +16)) {
					giantAlienBulletArray[i]->Hit();
					playerShip->Hit();
					mmEffectEx(&player_hit);   // Play the player hit sound effect
				}
			}
			// Detect collisions between the giant aliens and the barrier sprites
			for (int k = 0; k < GIANT_ALIEN_ARRAY; k++){
			
				if (giantAlienArray[i]->IsAlive() && barrierArray[k]->IsAlive()) {
				
					if ((giantAlienArray[i]->GetPosX() >= barrierArray[k]->GetPosX() && giantAlienArray[i]->GetPosX() <= (barrierArray[k]->GetPosX() +32) 
					&& giantAlienArray[i]->GetPosY() +64 >= barrierArray[k]->GetPosY() && giantAlienArray[i]->GetPosY() +64 <= (barrierArray[k]->GetPosY() +32))
					|| (giantAlienArray[i]->GetPosX() +64 >= barrierArray[k]->GetPosX() && giantAlienArray[i]->GetPosX() +64 <= (barrierArray[k]->GetPosX() +32) 
					&& giantAlienArray[i]->GetPosY() +64 >= barrierArray[k]->GetPosY() && giantAlienArray[i]->GetPosY() +64 <= (barrierArray[k]->GetPosY() +32))) {
						
						for (int l = 0; l < 6; l++){
							barrierArray[k]->Hit();
							giantAlienArray[i]->Hit();
						}
						
						alienNumber -= 1;
						playerShip->Hit();
						mmEffectEx(&alien_explode);   // Play the alien explode sound effect
					}
				}
			}
			// Detect collisions between the giant aliens and the players spaceship sprite
			if (giantAlienArray[i]->IsAlive()) {
			
				if ((giantAlienArray[i]->GetPosX() >= playerShip->GetPosX() && giantAlienArray[i]->GetPosX() <= (playerShip->GetPosX() +16)
				&& giantAlienArray[i]->GetPosY() +64 >= playerShip->GetPosY() && giantAlienArray[i]->GetPosY() +64 <= (playerShip->GetPosY() +16))
				|| (giantAlienArray[i]->GetPosX() +64 >= playerShip->GetPosX() && giantAlienArray[i]->GetPosX() +64 <= (playerShip->GetPosX() +16) 
				&& giantAlienArray[i]->GetPosY() +64 >= playerShip->GetPosY() && giantAlienArray[i]->GetPosY() +64 <= (playerShip->GetPosY() +16))) {
					
					for (int l = 0; l < 6; l++){giantAlienArray[i]->Hit();}
					alienNumber -= 1;
					playerShip->Hit();
					mmEffectEx(&player_hit);   // Play the player hit sound effect
				}
			}
			// Detect if any giant aliens reach the upper bounds of the screen (bottom of screen in game)
			if (giantAlienArray[i]->IsAlive()) {
			
				if (giantAlienArray[i]->GetPosY() >= SCREEN_HEIGHT - 60) {
					
					for (int l = 0; l < 6; l++){giantAlienArray[i]->Hit();}
					alienNumber -= 1;
					playerShip->Hit();
					mmEffectEx(&player_hit);   // Play the player hit sound effect
				}	
			}	
		}
	}
}



void Game::HandleInput(){

	switch (gameState)																			// Switch Game State
	{
	//********************************** Game Menu Code **************************************************************************************
	  case GSM_MENU :																			// GAMESTATE main menu screen
	  {
		if(KEY_DOWN & keysDown()){																// If the DOWN button is pressed

			switch (menuState)																	// Switch Game State
			{
			  case MSM_MENU_ARCADE :															// MENUSTATE (Arcade mode option)
			  {
				NF_CreateTiledBg(1, 3, "Menu2");												// Create the top screen background (Highlight 2 player mode)
				menuState = MSM_MENU_CONTROLS;													// Set new menu state
				NF_CreateTiledBg(0, 3, "TopSplash");
				mmEffectEx(&menu_beep);
			  }
			  break;
			  case MSM_MENU_CONTROLS :															// MENUSTATE (High score option)
			  {			
				NF_CreateTiledBg(1, 3, "Menu3");												// Create the top screen background (Highlight 2 player mode)
				menuState = MSM_MENU_HIGHSCORE;													// Set new menu state
				NF_CreateTiledBg(0, 3, "TopSplash");
				mmEffectEx(&menu_beep);
			  }
			  break;
			  case MSM_MENU_HIGHSCORE :															// MENUSTATE (High score option)
			  {			
				NF_CreateTiledBg(1, 3, "Menu4");												// Create the top screen background (Highlight 2 player mode)
				menuState = MSM_MENU_QUIT;														// Set new menu state
				NF_CreateTiledBg(0, 3, "TopSplash");
				mmEffectEx(&menu_beep);
				
				if (highScoreDisplay) {
					highScoreDisplay = false;
					NF_DeleteTextLayer(0,0); 													// Delete text layer
				} 
			  }
			  break;
			  case MSM_MENU_QUIT :																// MENUSTATE (Quit game option)
			  {}
			  break;
			}
		}
		if(KEY_UP & keysDown()){																// If the UP button is pressed
		
			switch (menuState)																	// Switch Game State
			{
			  case MSM_MENU_ARCADE :															// MENUSTATE (Arcade mode option)
			  {}
			  break;
			  case MSM_MENU_CONTROLS :															// MENUSTATE (High score option)
			  {			
				NF_CreateTiledBg(1, 3, "Menu1");												// Create the top screen background (Highlight 1 player mode)
				menuState = MSM_MENU_ARCADE;													// Set new menu state
				NF_CreateTiledBg(0, 3, "TopSplash");
				mmEffectEx(&menu_beep);
			  }
			  break;
			  case MSM_MENU_HIGHSCORE :															// MENUSTATE (High score option)
			  {			
				NF_CreateTiledBg(1, 3, "Menu2");												// Create the top screen background (Highlight 1 player mode)
				menuState = MSM_MENU_CONTROLS;													// Set new menu state
				NF_CreateTiledBg(0, 3, "TopSplash");
				mmEffectEx(&menu_beep);
				
				if (highScoreDisplay) {
					highScoreDisplay = false;
					NF_DeleteTextLayer(0,0); 													// Delete text layer
				} 
			  }
			  break;
			  case MSM_MENU_QUIT :																// MENUSTATE (Quit game option)
			  {
				NF_CreateTiledBg(1, 3, "Menu3");												// Create the top screen background (Highlight 2 player mode)
				menuState = MSM_MENU_HIGHSCORE;													// Set new menu state
				NF_CreateTiledBg(0, 3, "TopSplash");
				mmEffectEx(&menu_beep);
			  }
			  break;
			}
		}
		if(KEY_A & keysDown()){																	// If the B button is pressed
			
			if (menuState == MSM_MENU_ARCADE){
				NF_LoadTiledBg("bg/Top_Game_Screen", "TopGameScreen", 256, 256);		  		// Load a Background into RAM for menu item 1
				NF_LoadTiledBg("bg/Bottom_Game_Screen", "BottomGameScreen", 256, 256);	  		// Load a Background into RAM for menu item 2
				NF_CreateTiledBg(0, 3, "TopGameScreen");							  			// Create the Top Background
				
				// Stop the menu background music and start the main game background music
				mmStop();
				mmSetModuleVolume(550); 														// Raise audio volume for main game audio
				mmStart( MOD_MAIN_BACKGROUND, MM_PLAY_LOOP );
				
				LevelUpdate();
				NF_CreateTiledBg(1, 3, "BottomGameScreen");						  				// Create the Bottom Background
			
				SetupGameMode();																// Initialise the game
				gameState = GSM_SINGLEPLAYER;													// If 1 player mode is currently highlighted
			} else if (menuState == MSM_MENU_CONTROLS){
				NF_CreateTiledBg(0, 3, "ControlScreen");							  			// Create the controls screen
			} else if (menuState == MSM_MENU_HIGHSCORE){	
				NF_CreateTiledBg(0, 3, "HighScoresScreen");							  			// Create the high scores screen
				DisplayHighScore();
			} else {
				NF_ResetTiledBgBuffers();
				NF_ResetSpriteBuffers();
				exit(0);
			}
		}
	  }
	  break;
	  case GSM_SINGLEPLAYER :																	// GAMESTATE single player mode
	  {	
		if(KEY_LEFT & keysHeld()){MovePlayer(0);}												// If the LEFT button is held, move spaceship left
		if(KEY_RIGHT & keysHeld()){MovePlayer(1);}												// If the RIGHT button is held, move spaceship right
		if(KEY_Y & keysDown() || KEY_R & keysDown()){PlayerShoot();}							// If the Y button is pressed, the player shoots a bullet
		
		if(KEY_X & keysDown()){
																
			ResetGame();
			menuState = MSM_MENU_ARCADE;																									
			gameState = GSM_MENU;                   																						
			InitialiseMainMenu();
			mmStop();
			PlayGame();																			// Stop the main game background music
		}	

		// Pause game
		if(KEY_START & keysHeld()){
		
			if (gamePaused == false) {
			
				gameState = GSM_PAUSE;
				gamePaused = true;
				mmPause();
				
				NF_CreateSprite(1, 105, 105, 1, (SCREEN_WIDTH / 2) - 32, (SCREEN_HEIGHT / 2) - 16);									// Draws the alien Sprite in the designated spot
				NF_SpriteFrame(1, 105, 0);
				NF_ShowSprite(1, 105, true);
			}
		}
		
		// Nuke all aliens //////////////////////////// REMOVE!!! /////////////////////////////////////////////
		if(KEY_L & keysDown()){Nuke();}
													
	  }
	  break;
	  case GSM_END :																			// GAMESTATE game end mode
	  {
		if(KEY_X & keysDown()){																	// If the X button is pressed, clear the playing field
			ResetGame();
	
			menuState = MSM_MENU_ARCADE;																									
			gameState = GSM_MENU;   
			InitialiseMainMenu();
			PlayGame();
		}	
	  }
	  break;
	  case GSM_PAUSE :																			// GAMESTATE game pause mode
	  {	
		if(KEY_SELECT & keysHeld()){
		
			if (gamePaused) {
			
				gameState = GSM_SINGLEPLAYER;
				gamePaused = false;
				mmResume();
				NF_ShowSprite(1, 105, false);
			}
		}
	  }
	  break;
	}	
}


void Game::PlayGame() {

	touchPosition Stylus;	  																	// Prepare a variable for Stylus data

	mmStart( MOD_MENU_BACKGROUND, MM_PLAY_LOOP );												// Start playing menu background music
	mmSetModuleVolume(100);

	while(1) {
		scanKeys();		       																	// Scan the keys for Input
		touchRead(&Stylus);	   																	// Read Stylus data
	
		switch (gameState){																		// Switch Game State
		//********************************** Game Menu Code **************************************************************************************
		case GSM_MENU :																			// GAMESTATE main menu screen
		{
			HandleInput();
		}
		break;
		//******************************** Game Playing Code ***********************************************************************************
		case GSM_SINGLEPLAYER :																	// GAMESTATE single player mode
		{			
			HandleInput();	
			UpdateAliens();
			playerShip->Update();
			UpdatePlayerBullet();
			DetectCollision();
			UpdateGameText();
			NF_UpdateTextLayers();																// Update the text layers
			if (ufo->IsAlive()) {ufo->Update();}
			CheckGameEnd();
		}
		break;
		//******************************** Game end Code ***************************************************************************************
		case GSM_END :																			// GAMESTATE game end mode
		{
			HandleInput();
		}
		break;
		case GSM_PAUSE :																		// GAMESTATE game pause mode
		{
			HandleInput();
		}
		break;
		}
		//****************************************************************************************************************************************
		NF_SpriteOamSet(1);																		// Update NFLib's Sprite OAM System
		swiWaitForVBlank();																		// Wait for the Vertical Blank
		oamUpdate(&oamSub);																		// Update the OAM of the Bottom Screen engine
	}
}


void Game::DisplayHighScore() {

	// Attempt to access the filesystem and read from the file
	if (fatInitDefault()) {
	
		// Gets the current name of the rom in order to create the sav file
		char saveName[256] = { '\0' };
		if(_argc > 0 && strlen(_argv[0]) > 4) {
			snprintf(saveName, 255, "%.*s.sav", strlen(_argv[0]) - 4, _argv[0]);
		}
		else {
			sprintf(saveName, "game.sav");
		}
		
		// check if the file already exists
		FILE* inFile = fopen(saveName,"rb");
		if(inFile == NULL) {
		
			// If the file doesnt already exist,try and create it
			FILE * outFile = fopen (saveName,"wb");
			if(outFile!=NULL) {
			
				fputs ("0011100222003330044400555AAABBBCCCDDDEEE", outFile); // Write initial high scores to file
				fclose (outFile);
			}
		}

		fclose(inFile); // Close the file
	
		// These hold the scores that will be read from the file
		int score1 = 0, score2 = 0, score3 = 0, score4 = 0, score5 = 0;
	
		// Try and open the file for reading
		FILE* scoreFile = fopen(saveName,"rb");
		if(scoreFile) {
		
			highScoreDisplay = true;
		
			// This is for reading all of the scores from the file
			uint len;
			fseek(scoreFile,0,SEEK_END);
			len = ftell(scoreFile);        	// Get the length of the file in bytes (Should be 25 bytes)
			fseek(scoreFile,0,SEEK_SET);	// Return to first byte in file
			
			{
				char *entireFile = (char*)malloc(len+1);
				entireFile[len] = 0;
				
				if(fread(entireFile,1,len,scoreFile) != len) {
					//iprintf("error reading the bytes from the file!\n");
				}
					
				for(int i=0; i<5; i++) score1 = 10*score1 + entireFile[i]-'0'; 		// Convert array of char to a single int for high score 1
				for(int i=5; i<10; i++) score2 = 10*score2 + entireFile[i]-'0'; 	// Convert array of char to a single int for high score 2	
				for(int i=10; i<15; i++) score3 = 10*score3 + entireFile[i]-'0'; 	// Convert array of char to a single int for high score 3	
				for(int i=15; i<20; i++) score4 = 10*score4 + entireFile[i]-'0'; 	// Convert array of char to a single int for high score 4	
				for(int i=20; i<25; i++) score5 = 10*score5 + entireFile[i]-'0'; 	// Convert array of char to a single int for high score 5	

				free(entireFile); // Clear the char array
			}

			// This is for reading the name 1
			len = 3;        
			fseek(scoreFile,25,SEEK_SET);	

			char *name1 = (char*)malloc(len+1);
			name1[len] = 0;
			
			if(fread(name1,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for reading the name 2
			len = 3;        
			fseek(scoreFile,28,SEEK_SET);	

			char *name2 = (char*)malloc(len+1);
			name2[len] = 0;
			
			if(fread(name2,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for reading the name 3
			len = 3;        
			fseek(scoreFile,31,SEEK_SET);	

			char *name3 = (char*)malloc(len+1);
			name3[len] = 0;
			
			if(fread(name3,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for reading the name 4
			len = 3;        
			fseek(scoreFile,34,SEEK_SET);	

			char *name4 = (char*)malloc(len+1);
			name4[len] = 0;
			
			if(fread(name4,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for reading the name 5
			len = 3;        
			fseek(scoreFile,37,SEEK_SET);	

			char *name5 = (char*)malloc(len+1);
			name5[len] = 0;
			
			if(fread(name5,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			fclose(scoreFile); // Close the file
			
			// Display the data that has been read from the text file
			u8 scoreLength = 5;
			char blank[0];
				
			// Convert the integers to char arrays
			char score_1[scoreLength];
			sprintf(score_1, "%d", score1);	
			
			char score_2[scoreLength];
			sprintf(score_2, "%d", score2);	
			
			char score_3[scoreLength];
			sprintf(score_3, "%d", score3);	
			
			char score_4[scoreLength];
			sprintf(score_4, "%d", score4);	
			
			char score_5[scoreLength];
			sprintf(score_5, "%d", score5);	

			NF_SetRootFolder("NITROFS");  																									// Set the Root Directory to NITRO FS
			NF_InitTextSys(0);	                                     																		// Initialize text system on top screen
			NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);	 																		// Load font
			NF_CreateTextLayer(0, 0, 0,	"normal");				     																		// Normal (layer 0)
			
			//NF_DefineTextColor(0, 0, 1, 30, 0, 0);
			//NF_SetTextColor(0, 0, 1);
			
			// Scores
			NF_WriteText(0, 0, 12, 6, score_5);			 		
			NF_WriteText(0, 0, 12, 8, score_4);			 		
			NF_WriteText(0, 0, 12, 10, score_3);	
			NF_WriteText(0, 0, 12, 12, score_2);	
			NF_WriteText(0, 0, 12, 14, score_1);	

			// Names
			NF_WriteText(0, 0, 18, 6, name5);			 		
			NF_WriteText(0, 0, 18, 8, name4);			 		
			NF_WriteText(0, 0, 18, 10, name3);	
			NF_WriteText(0, 0, 18, 12, name2);	
			NF_WriteText(0, 0, 18, 14, name1);	

			// Blank text
			NF_CreateTextLayer(0, 0, 0,	"normal");				     																		
			NF_WriteText(0, 0, 9, 25, blank);	

			// Stop the main game background music and start the menu background music
			mmStop();
			mmStart( MOD_MENU_BACKGROUND, MM_PLAY_LOOP );																					// Start playing menu background music
			mmSetModuleVolume(100);		
		} else {
			consoleDemoInit();
			iprintf("There was a problem loading the file!\n");
		}
	} else {
		consoleDemoInit();
		iprintf("Error accessing filesystem!\n\n");
		iprintf("Try applying a DLDI patch!\n\n");
	}
}






void Game::CheckHighScore(int score) {

	// Attempt to access the filesystem and read from the file
	if (fatInitDefault()) {
	
		// Gets the current name of the rom in order to create the sav file
		char saveName[256] = { '\0' };
		if(_argc > 0 && strlen(_argv[0]) > 4) {
			snprintf(saveName, 255, "%.*s.sav", strlen(_argv[0]) - 4, _argv[0]);
		}
		else {
			sprintf(saveName, "game.sav");
		}
	
		// check if the file already exists
		FILE* inFile = fopen(saveName,"rb");
		if(inFile == NULL) {
		
			// If the file doesnt already exist,this attempts to create one
			FILE * outFile = fopen (saveName,"wb");
			if(outFile!=NULL) {
				fputs ("0011100222003330044400555AAABBBCCCDDDEEE", outFile); // Write initial high scores to file
				fclose (outFile);
			}
		}

		fclose(inFile); // Close the file
	
		// These hold the scores that will be read from the file
		int score1 = 0, score2 = 0, score3 = 0, score4 = 0, score5 = 0;
	
		// Try and open the file for reading
		FILE* scoreFile = fopen(saveName,"rb");
		if(scoreFile) {	
	
			uint len;
			fseek(scoreFile,0,SEEK_END);
			len = ftell(scoreFile);        	// Get the length of the file in bytes (Should be 40 bytes)
			fseek(scoreFile,0,SEEK_SET);	// Return to first byte in file
			
			// This is for storing the high scores as integers
			{
				char *entireFile = (char*)malloc(len+1);
				entireFile[len] = 0;
				
				// Stores each byte from the file into the entireFile char array
				if(fread(entireFile,1,len,scoreFile) != len) {
					consoleDemoInit();
					iprintf("error reading the bytes from the file!\n");
				}
					
				 // Converts each of the high scores to integers for performing calculations
				for(int i=0; i<5; i++) score1 = 10*score1 + entireFile[i]-'0';	
				for(int i=5; i<10; i++) score2 = 10*score2 + entireFile[i]-'0';		
				for(int i=10; i<15; i++) score3 = 10*score3 + entireFile[i]-'0';		
				for(int i=15; i<20; i++) score4 = 10*score4 + entireFile[i]-'0';	
				for(int i=20; i<25; i++) score5 = 10*score5 + entireFile[i]-'0';
				
				free(entireFile); // Clear the char array
			}

			// This is for storing high score 1 into a char array
			len = 5;
			fseek(scoreFile,0,SEEK_SET);	

			char *highScore1 = (char*)malloc(len+1);
			highScore1[len] = 0;
			
			if(fread(highScore1,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for storing high score 2 into a char array
			fseek(scoreFile,5,SEEK_SET);	

			char *highScore2 = (char*)malloc(len+1);
			highScore2[len] = 0;
			
			if(fread(highScore2,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for storing high score 3 into a char array
			fseek(scoreFile,10,SEEK_SET);	

			char *highScore3 = (char*)malloc(len+1);
			highScore3[len] = 0;
			
			if(fread(highScore3,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for storing high score 4 into a char array
			fseek(scoreFile,15,SEEK_SET);	

			char *highScore4 = (char*)malloc(len+1);
			highScore4[len] = 0;
			
			if(fread(highScore4,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for storing high score 5 into a char array
			fseek(scoreFile,20,SEEK_SET);	

			char *highScore5 = (char*)malloc(len+1);
			highScore5[len] = 0;
			
			if(fread(highScore5,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for reading the name 1
			len = 3;        
			fseek(scoreFile,25,SEEK_SET);	

			char *name1 = (char*)malloc(len+1);
			name1[len] = 0;
			
			if(fread(name1,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for reading the name 2 
			fseek(scoreFile,28,SEEK_SET);	

			char *name2 = (char*)malloc(len+1);
			name2[len] = 0;
			
			if(fread(name2,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for reading the name 3       
			fseek(scoreFile,31,SEEK_SET);	

			char *name3 = (char*)malloc(len+1);
			name3[len] = 0;
			
			if(fread(name3,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for reading the name 4       
			fseek(scoreFile,34,SEEK_SET);	

			
			char *name4 = (char*)malloc(len+1);
			name4[len] = 0;
			
			if(fread(name4,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			// This is for reading the name 5      
			fseek(scoreFile,37,SEEK_SET);	

			
			char *name5 = (char*)malloc(len+1);
			name5[len] = 0;
			
			if(fread(name5,1,len,scoreFile) != len) {
				consoleDemoInit();
				iprintf("Error reading the file!\n");
			}
			
			fclose(scoreFile); // Close the file
			
			// Compare the players score
			u8 highScore = 0;
			
			if (score > score5) {highScore = 5;} 
			else if (score > score4) {highScore = 4;} 
			else if (score > score3) {highScore = 3;} 
			else if (score > score2) {highScore = 2;} 
			else if (score > score1) {highScore = 1;}
			
			// Gets the length of the players score
			int scoreLength = 1;
			int playerScore = score;
			if (playerScore > 0) {for (scoreLength = 0; playerScore > 0; scoreLength++) {playerScore = playerScore / 10;}}
		
			// This pads the score with leading zero's if it is less than 5 digits
			int num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0;
			if (scoreLength >= 1 && scoreLength <= 5) {
				num1 = score / 10000 % 10;
				num2 = score / 1000 % 10;
				num3 = score / 100 % 10;
				num4 = score / 10 % 10;
				num5 = score % 10;
			}
			
			// Char array to store the players score as 5 digits
			char scoreBuffer[5];
			sprintf(scoreBuffer, "%d%d%d%d%d", num1,num2,num3,num4,num5);

			// If the player has achieved a high score
			if (highScore != 0) {

				consoleDemoInit();  										//setup the sub screen for printing
				keyboardDemoInit(); 										// Initialise the virtual keyboard
				keyboardShow();												// Display the virtual keyboard
				
				char name_1 = 'A';
				char name_2 = 'A';
				char name_3 = 'A';
					
				int keyPresses = 0;											// Counts the keypresses on the virtual keyboard
				
				while(keyPresses < 3) {
				
					int key = keyboardUpdate();								// gets the keypress from the virtual keyboard

					if(key > 0 && keyPresses == 0){

						char charBuffer[1];
						sprintf(charBuffer, "%c", key);						// Convert the keypress to a char
						
						if (isalpha(charBuffer[0])) {						// Check if the char is alphabetical
						
							iprintf("%c", key);								// prints the char on the screen
							name_1 = key;									// Stores the char
							keyPresses ++;									// increments the keypress counter
						}
						//free(charBuffer);
					} 
					else if(key > 0 && keyPresses == 1){
					
						char charBuffer[1];
						sprintf(charBuffer, "%c", key);						// Convert the keypress to a char
						
						if (isalpha(charBuffer[0])) {						// Check if the char is alphabetical
						
							iprintf("%c", key);								// prints the char on the screen
							name_2 = key;									// Stores the char
							keyPresses ++;									// increments the keypress counter
						}
						//free(charBuffer);
					} 
					else if(key > 0 && keyPresses == 2){

						char charBuffer[1];
						sprintf(charBuffer, "%c", key);						// Convert the keypress to a char
						
						if (isalpha(charBuffer[0])) {						// Check if the char is alphabetical
						
							iprintf("%c", key);								// prints the char on the screen
							name_3 = key;									// Stores the char
							keyPresses ++;									// increments the keypress counter
						}
						//free(charBuffer);
					}
					
					if (keyPresses == 3) {keyboardHide();}					// Once 3 alphabetical characters have been selected the virtual keyboard is hidden
				}
				
				// Char array for the user's name
				char nameBuffer[3];
				sprintf(nameBuffer, "%c%c%c", name_1,name_2,name_3);

				// Try and open the file for writing
				scoreFile = fopen(saveName,"rb+");
				if(scoreFile) {
			
					// Write the new high score to the specific location in the file and update the other high scores
					if (highScore == 1) {
						fseek(scoreFile,0,SEEK_SET);
						fwrite (scoreBuffer , sizeof(char), sizeof(scoreBuffer), scoreFile);
					}
					else if (highScore == 2) {
					
						fseek(scoreFile,0,SEEK_SET);
						fwrite (highScore2 , sizeof(char), sizeof(highScore2), scoreFile);
						
						fseek(scoreFile,5,SEEK_SET);
						fwrite (scoreBuffer , sizeof(char), sizeof(scoreBuffer), scoreFile);
					}
					else if (highScore == 3) {
					
						fseek(scoreFile,0,SEEK_SET);
						fwrite (highScore2 , sizeof(char), sizeof(highScore2), scoreFile);
					
						fseek(scoreFile,5,SEEK_SET);
						fwrite (highScore3 , sizeof(char), sizeof(highScore3), scoreFile);
						
						fseek(scoreFile,10,SEEK_SET);
						fwrite (scoreBuffer , sizeof(char), sizeof(scoreBuffer), scoreFile);
						
					}
					else if (highScore == 4) {
					
						fseek(scoreFile,0,SEEK_SET);
						fwrite (highScore2 , sizeof(char), sizeof(highScore2), scoreFile);
					
						fseek(scoreFile,5,SEEK_SET);
						fwrite (highScore3 , sizeof(char), sizeof(highScore3), scoreFile);
					
						fseek(scoreFile,10,SEEK_SET);
						fwrite (highScore4 , sizeof(char), sizeof(highScore4), scoreFile);
					
						fseek(scoreFile,15,SEEK_SET);
						fwrite (scoreBuffer , sizeof(char), sizeof(scoreBuffer), scoreFile);
						
					}
					else if (highScore == 5) {
					
						fseek(scoreFile,0,SEEK_SET);
						fwrite (highScore2 , sizeof(char), sizeof(highScore2), scoreFile);
						
						fseek(scoreFile,5,SEEK_SET);
						fwrite (highScore3 , sizeof(char), sizeof(highScore3), scoreFile);
						
						fseek(scoreFile,10,SEEK_SET);
						fwrite (highScore4 , sizeof(char), sizeof(highScore4), scoreFile);
						
						fseek(scoreFile,15,SEEK_SET);
						fwrite (highScore5 , sizeof(char), sizeof(highScore5), scoreFile);
						
						fseek(scoreFile,20,SEEK_SET);
						fwrite (scoreBuffer , sizeof(char), sizeof(scoreBuffer), scoreFile);
					}
				
				
					// Write the users name to the specific location in the file and update the other names
					if (highScore == 1) {
						fseek(scoreFile,25,SEEK_SET);
						fwrite (nameBuffer , sizeof(char), sizeof(nameBuffer), scoreFile);
					}
					else if (highScore == 2) {
					
						fseek(scoreFile,25,SEEK_SET);
						fwrite (name2 , sizeof(char), sizeof(name2), scoreFile);
					
						fseek(scoreFile,28,SEEK_SET);
						fwrite (nameBuffer , sizeof(char), sizeof(nameBuffer), scoreFile);
					}
					else if (highScore == 3) {
					
						fseek(scoreFile,25,SEEK_SET);
						fwrite (name2 , sizeof(char), sizeof(name2), scoreFile);
					
						fseek(scoreFile,28,SEEK_SET);
						fwrite (name3 , sizeof(char), sizeof(name3), scoreFile);
						
						fseek(scoreFile,31,SEEK_SET);
						fwrite (nameBuffer , sizeof(char), sizeof(nameBuffer), scoreFile);
					}
					else if (highScore == 4) {
					
						fseek(scoreFile,25,SEEK_SET);
						fwrite (name2 , sizeof(char), sizeof(name2), scoreFile);
					
						fseek(scoreFile,28,SEEK_SET);
						fwrite (name3 , sizeof(char), sizeof(name3), scoreFile);
					
						fseek(scoreFile,31,SEEK_SET);
						fwrite (name4 , sizeof(char), sizeof(name4), scoreFile);
					
						fseek(scoreFile,34,SEEK_SET);
						fwrite (nameBuffer , sizeof(char), sizeof(nameBuffer), scoreFile);
					}
					else if (highScore == 5) {
					
						fseek(scoreFile,25,SEEK_SET);
						fwrite (name2 , sizeof(char), sizeof(name2), scoreFile);
					
						fseek(scoreFile,28,SEEK_SET);
						fwrite (name3 , sizeof(char), sizeof(name3), scoreFile);
					
						fseek(scoreFile,31,SEEK_SET);
						fwrite (name4 , sizeof(char), sizeof(name4), scoreFile);
					
						fseek(scoreFile,34,SEEK_SET);
						fwrite (name5 , sizeof(char), sizeof(name5), scoreFile);

						fseek(scoreFile,37,SEEK_SET);
						fwrite (nameBuffer , sizeof(char), sizeof(nameBuffer), scoreFile);	
					}

					fclose (scoreFile); // Close the file
				}
				
				/*
				// Clear the char arrays
				free(highScore1Buffer);
				free(highScore2Buffer);
				free(highScore3Buffer);
				free(highScore4Buffer);
				free(highScore5Buffer);
				free(name1);
				free(name2);
				free(name3);
				free(name4);
				free(name5);
				free(scoreBuffer);
				free(nameBuffer);
				*/
			}

			// Reset the bottom screen to the main menu
			ResetGame();
			
			NF_SetRootFolder("NITROFS"); 
			
			InitialiseMainMenu();
																		
			NF_CreateTiledBg(0, 3, "HighScoresScreen");																							
			NF_CreateTiledBg(1, 3, "Menu3");																								
			gameState = GSM_MENU;
			menuState = MSM_MENU_HIGHSCORE;	
																
			DisplayHighScore();
				
		} else {
			consoleDemoInit();
			iprintf("There was a problem loading the file!\n");
		}
	} else {
		consoleDemoInit();
		iprintf("Error accessing filesystem!\n\n");
		iprintf("Try applying a DLDI patch!\n\n");
	}
}



void Game::Nuke() {

	for (int i = 0; i < ALIEN_ROWS; i++){
		for (int j = 0; j < ALIEN_COLUMNS; j++){
				
			// Kill all aliens at once
			alienArray[i][j]->Hit();
			alienNumber = 0;
		}
	}
}
