/*
 *  Bullet.cpp
 *  
 *  Created by LoGi on 02/07/2015
 *  Copyright 2015 All rights reserved
 */

#include "bullet.h"

// Bullet constructor
Bullet::Bullet(float x, float y, u8 id, u8 size) {
	
	_position_X = x;
	_position_Y = y;
	_id = id;
	_size = size;
	
	if (_size == 0) {
		_width = 8;
		_height = 8;
	} else if (_size == 1) {
		_width = 8;
		_height = 16;
	}
	
	_speed = 3;     // 3
	_alive = false;
	_direction = 0;
	_frame = 0;
}


// Bullet deconstructor
Bullet::~Bullet() {

}


// Moves the bullet up the screen
void Bullet::MoveUp() {

	if (_position_Y  > 0 - _height) {_position_Y -= _speed;} else {_alive = false;}
	NF_MoveSprite(1, _id, _position_X, _position_Y);  																				
}


// Moves the bullet down the screen
void Bullet::MoveDown() {

	if (_position_Y  < SCREEN_HEIGHT + _height) {_position_Y += _speed;} else {_alive = false;}
	NF_MoveSprite(1, _id, _position_X, _position_Y);  																				
}


void Bullet::Load() {

	if (_size == 0) {
		NF_LoadSpriteGfx("sprite/Spaceship_Bullet", 4, 8, 8);  																		// Loads the image for the bullet sprite
		NF_VramSpriteGfx(1, 4, 4, false);						  																		// Loads the bullet sprite into VRAM
	} else if (_size == 1) {
		NF_LoadSpriteGfx("sprite/Large_Bullet", 104, 8, 16);  																			// Loads the image for the bullet sprite
		NF_VramSpriteGfx(1, 104, 104, false);						  																	// Loads the bullet sprite into VRAM
	}
}


void Bullet::Draw() {

	if (_size == 0) {
		NF_SpriteLayer(1, _id, 2);
		NF_CreateSprite(1, _id, 4, 0, _position_X, _position_Y);  																	// Draws the player bullet Sprite
		NF_SpriteFrame(1, _id, _frame);	
		NF_ShowSprite(1,_id,true);		  																							// Sets the sprites current frame to frame 0
	} else if (_size == 1) {
		NF_SpriteLayer(1, _id, 2);
		NF_CreateSprite(1, _id, 104, 0, _position_X, _position_Y);  																// Draws the player bullet Sprite
		NF_SpriteFrame(1, _id, _frame);			  																					// Sets the sprites current frame to frame 0	
		NF_ShowSprite(1,_id,true);																			
	}
}

void Bullet::Hit() {
	_alive = false;
	NF_ShowSprite(1,_id,false);
}


void Bullet::Reset() {
	_alive = false;
	_frame = 0;		  																												// Sets the sprites current frame to frame 1 (blank frame)
}


// Gets the float representing the bullet's X position
const float& Bullet::GetPosX() {return _position_X;}

// Sets the float representing the bullet's X position
void Bullet::SetPosX(float position) {_position_X = position;}


// Gets the float representing the bullet's Y position
const float& Bullet::GetPosY() {return _position_Y;}

// Sets the float representing the bullet's Y position
void Bullet::SetPosY(float position) {_position_Y = position;}


// Gets the bool representing the bullet's ID
const bool& Bullet::IsAlive() {return _alive;}	
	
// Sets the u8 representing the bullet's ID
void Bullet::SetAlive(bool alive) {_alive = alive;}
