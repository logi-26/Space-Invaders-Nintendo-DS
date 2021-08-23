/*
 *  Alien.cpp
 *  
 *  Created by LoGi on 02/07/2015
 *  Copyright 2015 All rights reserved
 */

#include "alien.h"

// Alien constructor
Alien::Alien(float x, float y, u8 id, float shootPosition, float speed, u8 size) {
	
	_position_X = x;
	_position_Y = y;
	_id = id;
	_shootPosition = shootPosition;
	_speed = speed;
	_alienSize = size;
	
	if (_alienSize == 0) {_width = 16, _height = 16;}
	else if (_alienSize == 1) {_width = 32, _height = 32;}
	else if (_alienSize == 2) {_width = 64, _height = 64;}

	_alive = true;
	_hit = false;
	_hitCounter = 0;
	_direction = 0;
	_animationTimer = 0;
	_frame = 0;
	_points = 0;	
}


// Alien deconstructor
Alien::~Alien() {
	
}


// Move alien right
void Alien::MoveRight() {

	if (_position_X  < SCREEN_WIDTH - _width) {_position_X += _speed;}	
	NF_MoveSprite(1, _id, _position_X, _position_Y);
}



// Move alien left
void Alien::MoveLeft() {

	if (_position_X  > 0) {_position_X -= _speed;}
	NF_MoveSprite(1, _id, _position_X, _position_Y);
}


void Alien::Load() {

}


void Alien::Update() {

	if (_direction == 0) {MoveLeft();} else {MoveRight();}

	// This updates the sprite frame for the standard sized aliens
	if (_alienSize == 0 || _alienSize == 1) {

		// Sprite animation
		_animationTimer ++;
		
		if (_alive) {
			if (_animationTimer > 50) {
				_animationTimer = 0;
			
				if (_frame == 0) {_frame = 1;} else {_frame = 0;}
			}
		} else if (_frame != 2 && _frame < 3) {
			_animationTimer = 0;
			_frame = 2;
		} else {
			if (_animationTimer > 10) {
				_animationTimer = 0;
				_frame = 3;
			}	
		}
		
		NF_SpriteFrame(1, _id, _frame);
	
	// This updates the sprite frame for the giant sized aliens
	} else if (_alienSize == 2) {
	
		// Sprite animation
		_animationTimer ++;
		
		if (_alive && !_hit) {
		
			if (_animationTimer > 50) {
				_animationTimer = 0;
				
				if (_frame == 0) {_frame = 1;} else {_frame = 0;}
			}
		} 
		
		if (_alive && _hit && _frame != 2 && _frame < 3) {
			_animationTimer = 30;
			_frame = 2;
			_hit = false;
		} 
		
		if (!_alive) {
			if (_animationTimer < 50) {
				_frame = 2;
			} else {
				_frame = 3;
				_animationTimer = 50;
			}
		} 
	}	
	
	NF_SpriteFrame(1, _id, _frame);
}



void Alien::Draw(int alienType) {

	NF_SpriteLayer(1, _id, 2);
	NF_CreateSprite(1, _id, alienType, 1, _position_X, _position_Y);									// Draws the alien Sprite in the designated spot
	NF_SpriteFrame(1, _id, _frame);
}


void Alien::Hit() {

	if (_alienSize == 0 || _alienSize == 1) {_alive = false;}
	else if (_alienSize == 2) {
		if (_hitCounter < 4) {
			_hitCounter ++;
		} else {
			_alive = false;
		}
	}
	
	_hit = true;
}


void Alien::Reset() {

	_alive = true;
	_frame = 0;
	_direction = 0;
	_animationTimer = 0;
	_speed = 0.30;
}



// Gets the float representing the alien's X position
const float& Alien::GetPosX() {return _position_X;}

// Sets the float representing the alien's X position
void Alien::SetPosX(float position) {_position_X = position;}


// Gets the float representing the alien's Y position
const float& Alien::GetPosY() {return _position_Y;}

// Sets the float representing the alien's Y position
void Alien::SetPosY(float position) {_position_Y = position;}


// Gets the u8 representing the alien's current direction
const u8& Alien::GetDirection() {return _direction;}	
	
// Sets the u8 representing the alien's current direction
void Alien::SetDirection(u8 dir) {_direction = dir;}


// Gets the bool representing the alien's alive value
const bool& Alien::IsAlive() {return _alive;}	
	
	
// Sets the float representing the alien's shoot position
void Alien::SetShootPos(float position) {_shootPosition = position;}	
	
// Gets the float representing the alien's shoot position
const float& Alien::GetShootPosition() {return _shootPosition;}


// Gets the int representing the alien's points
const int& Alien::GetPoints() {return _points;}	
	
// Sets the int representing the alien's points
void Alien::SetPoints(int points) {_points = points;}
