/*
 *  Spaceship.cpp
 *  
 *  Created by LoGi on 02/07/2015
 *  Copyright 2015 All rights reserved
 */

#include "spaceship.h"

// Spaceship constructor
Spaceship::Spaceship() {
	
	_speed = 0.4;
	_width = 16;
	_height = 16;
	_score = 0;
	_lives = 3;
	_animationTimer = 0;
	_frame = 0;
	
	// Position the ship at the bottom of the screen in the centre
	_position_X = SCREEN_WIDTH / 2 - _width / 2;
	_position_Y = SCREEN_HEIGHT - _height + 5;
}

// Spaceship deconstructor
Spaceship::~Spaceship() {
	
}

// Moves the players spaceship sprite right
void Spaceship::MoveRight() {

	if (_position_X  < SCREEN_WIDTH - _width && _frame != 1) {_position_X += _speed;}	
	NF_MoveSprite(1, 0, _position_X, _position_Y); 
}

// Moves the players spaceship sprite left
void Spaceship::MoveLeft() {

	if (_position_X  > 0 && _frame != 1) {_position_X -= _speed;}
	NF_MoveSprite(1, 0, _position_X, _position_Y);
}

// Updates the sprite frames
void Spaceship::Update() {

	_animationTimer ++;

	if (_lives >= 0 && _lives < 200) {
		if (_animationTimer > 20) {
			_animationTimer = 0;
		
			if (_frame == 1) {_frame = 0;}
		}
	} 
	
	NF_SpriteFrame(1, 0, _frame);																	// Displays the spaceships current frame
}

// Loads the sprite image data
void Spaceship::Load() {

	// Creates the players spaceship sprite
	NF_LoadSpriteGfx("sprite/Spaceship_Small", 0, 16, 16);  										// Loads the image for the spaceship sprite
	NF_LoadSpritePal("sprite/Spaceship_Small", 0);													// Loads the colour pallete for the spaceship sprite		
	NF_VramSpriteGfx(1, 0, 0, false);						  										// Loads the spaceship sprite into VRAM
	NF_VramSpritePal(1, 0, 0);								  										// Loads the spaceship sprites Palette into VRAM
	NF_SpriteFrame(1, 0, 0);			  															// Sets the sprites current frame to frame 0
	_animationTimer = 20;
}

// Draws the spaceship sprite
void Spaceship::Draw() {

	NF_SpriteLayer(1, 0, 2);
	NF_CreateSprite(1, 0, 0, 0, _position_X, _position_Y);  										// Draws the player spaceship Sprite in the designated spot
}

// Spaceship hit function
void Spaceship::Hit() {

	_lives -=1;													  									// Update the players lives
	_frame = 1;																						// Change the spaceships frame to show that it has been hit
	_animationTimer = 0;																			// Set the animation timer to zero
	NF_SpriteFrame(1, 0, _frame);																	// Switch the frame
}

// Resets the spaceship values
void Spaceship::Reset() {

	_position_X = SCREEN_WIDTH / 2 - _width / 2;
	_position_Y = SCREEN_HEIGHT - _height + 5;
	_lives = 3;
	_score = 0;
}

// Gets the float representing the spaceship's X position
const float& Spaceship::GetPosX() {return _position_X;}

// Gets the float representing the spaceship's Y position
const float& Spaceship::GetPosY() {return _position_Y;}

// Gets the u8 representing the spaceship's current frame
const u8& Spaceship::GetFrame() {return _frame;}

// Gets the int8 representing the players lives
const int8& Spaceship::GetPlayerLives() {return _lives;}

// Sets the int8 representing the players lives
void Spaceship::SetPlayerLives(int8 playerLives) {_lives = playerLives;}

// Gets the int representing the players score
const int& Spaceship::GetPlayerScore() {return _score;}

// Sets the int representing the players score
void Spaceship::SetPlayerScore(int playerScore) {_score = playerScore;}
