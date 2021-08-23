/*
 *  Alien.h
 *  
 *  Created by LoGi on 02/07/2015
 *  Copyright 2015 All rights reserved
 */

#ifndef ALIEN_H
#define ALIEN_H

#include <nds.h>		// Includes libnds
#include <nf_lib.h> 	// Includes NightFox Lib

using namespace std;

class Alien {

protected:

	float _position_X;
	float _position_Y;
	float _shootPosition;
    float _speed;
	int _points;
	u8 _id;
	u8 _width;
	u8 _height;
	u8 _frame;
	u8 _direction;
	u8 _alienSize;
	u8 _animationTimer;
	u8 _hitCounter;
	bool _hit;
	bool _alive;
 
	void MoveRight(); // Moves the alien right
	void MoveLeft(); // Moves the alien left
 
public:
	 
    // Alien constructor
    Alien(float x, float y, u8 id, float shootPosition, float speed, u8 size);

    // Alien destructor
    virtual ~Alien();
	 
	void Load();

	void Update();

	void Draw(int alienType);

	void Hit();

	void Reset();

    // Returns the float representing the alien's X position
	const float& GetPosX();
	
	// Set the float representing the alien's X position
	void SetPosX(float position);
	
	// Returns the float representing the alien's Y position
	const float& GetPosY();
	
	// Set the float representing the alien's Y position
	void SetPosY(float position);
	
	// Returns the u8 representing the alien's current direction
	const u8& GetDirection();
	
	// Set the u8 representing the alien's current direction
	void SetDirection(u8 direction);
	
	// Set the float representing the alien's shoot position
	void SetShootPos(float position);
	
	// Returns the bool representing the alien's alive variable
	const bool& IsAlive();
	
	// Returns the float representing the alien's shoot position
	const float& GetShootPosition();
	
	// Returns the int8 representing the alien's points
	const int& GetPoints();
	
	// Set the u8 representing the alien's points
	void SetPoints(int points);
};

#endif