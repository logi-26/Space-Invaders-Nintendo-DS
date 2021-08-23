/*
 *  UFO.cpp
 *  
 *  Created by LoGi on 12/07/2015
 *  Copyright 2015 All rights reserved
 */

#include "ufo.h"

// UFO constructor
Ufo::Ufo(float x, float y, u8 id) {
	
	_position_X = x;
	_position_Y = y;
	_id = id;
	
	_direction = 0;
	_speed = 1.2;  
	_width = 16;
	_height = 14;
	_frame = 0;
	_animationTimer = 0;
	_alive = false;
	_hit = false;
}

// Ufo deconstructor
Ufo::~Ufo() {
	
}

void Ufo::Load() {
	
	NF_LoadSpriteGfx("sprite/UFO", 100, 16, 16);  																						// Loads the image for the barrier sprite
	NF_VramSpriteGfx(1, 100, 100, false);						  																		// Loads the barrier sprite into VRAM
}

void Ufo::Draw() {
	
	_alive = true;
	_hit = false;
	_animationTimer = 0;
	_frame = 0;
	
	NF_SpriteLayer(1, _id, 2);
	NF_CreateSprite(1, _id, 100, 0, _position_X, _position_Y);  																		// Draws the barrier Sprite
	NF_SpriteFrame(1, _id, _frame);				  																						// Sets the sprites current frame to frame 0
}

void Ufo::Hit() {

	_hit = true;
}

void Ufo::Update() {

	// This moves the ufo across the screen depending on the direction
	if (_alive && _direction == 0 && !_hit) {
		_position_X -= _speed;
		NF_MoveSprite(1, _id, _position_X, _position_Y);
		
		if (_position_X < 0 - _width) {Hit();}
		
	} else if (_alive && _direction == 1 && !_hit) {
		_position_X += _speed;
		NF_MoveSprite(1, _id, _position_X, _position_Y);
		
		if (_position_X > 260) {Hit();}
	}
	
	// This performs the animation when the ufo is hit
	if (_hit) {
	
		// Sprite animation
		_animationTimer ++;
	
		if (_frame == 0) {
			_animationTimer = 0;
			_frame = 1;
		} else if (_frame == 1) {
		
			if (_animationTimer > 20) {
				_animationTimer = 0;
				_frame = 2;
				_alive = false;
			}	
		}
		
		NF_SpriteFrame(1, _id, _frame);
	}
}


// Gets the float representing the Ufo's X position
const float& Ufo::GetPosX() {return _position_X;}

// Sets the float representing the Ufo's X position
void Ufo::SetPosX(float position) {_position_X = position;}

// Gets the float representing the Ufo's Y position
const float& Ufo::GetPosY() {return _position_Y;}

// Sets the float representing the Ufo's Y position
void Ufo::SetPosY(float position) {_position_Y = position;}

// Sets the float representing the Ufo's speed
//void Ufo::SetSpeed(float speed) {_speed = speed;}

// Sets the float representing the Ufo's direction
void Ufo::SetDirection(u8 direction) {_direction = direction;}

// Gets the bool representing the Ufo's alive value
const bool& Ufo::IsAlive() {return _alive;}	