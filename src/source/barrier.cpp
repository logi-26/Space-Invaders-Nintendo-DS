/*
 *  Barrier.cpp
 *  
 *  Created by LoGi on 09/07/2015
 *  Copyright 2015 All rights reserved
 */

#include "barrier.h"

// Barrier constructor
Barrier::Barrier(float x, float y, u8 id) {
	
	_position_X = x;
	_position_Y = y;
	_id = id;
	
	_width = 32;
	_height = 32;
	_frame = 0;
	_alive = true;
}


// Barrier deconstructor
Barrier::~Barrier() {
	
}


void Barrier::Load() {
	
	NF_LoadSpriteGfx("sprite/Barrier", 5, 32, 32);  																				// Loads the image for the barrier sprite
	NF_VramSpriteGfx(1, 5, 5, false);						  																		// Loads the barrier sprite into VRAM
}


void Barrier::Draw() {
	
	NF_SpriteLayer(1, _id, 2);
	NF_CreateSprite(1, _id, 5, 1, _position_X, _position_Y);  																		// Draws the barrier Sprite
	NF_SpriteFrame(1, _id, _frame);			  																						// Sets the sprites current frame to frame 0
}

void Barrier::Hit() {

	if (_frame != 6){
		_frame += 1;
		NF_SpriteFrame(1, _id, _frame);
	
		if (_frame == 6){_alive = false;}
	} 
}

void Barrier::Reset() {

	_frame = 0;
	_alive = true;
}


// Gets the float representing the barrier's X position
const float& Barrier::GetPosX() {return _position_X;}

// Gets the float representing the barrier's Y position
const float& Barrier::GetPosY() {return _position_Y;}

// Gets the bool representing the barrier's alive value
const bool& Barrier::IsAlive() {return _alive;}	
