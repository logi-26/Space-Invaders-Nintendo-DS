/*
 *  Bullet.h
 *  
 *  Created by LoGi on 03/07/2015
 *  Copyright 2015 All rights reserved
 */

#ifndef BULLET_H
#define BULLET_H

#include <nds.h>		// Includes libnds
#include <nf_lib.h> 	// Includes NightFox Lib

using namespace std;

class Bullet {

protected:

	float _position_X;
	float _position_Y;
    float _speed;
	u8 _width;
	u8 _height;
	u8 _direction;
	u8 _id;
	u8 _size;
	u8 _frame;
	bool _alive;
	
public:
     
    // Bullet constructor
    Bullet(float x, float y, u8 id, u8 size);

    // Bullet destructor
    virtual ~Bullet();
	 
	// Moves the bullet up
	void MoveUp();
	 
	// Moves the bullet down
	void MoveDown();
	 
	void Load();
	
	void Draw();
	
	void Hit();
	 
	void Reset();
	 
    // Returns the float representing the bullet's X position
	const float& GetPosX();
	
	// Returns the float representing the bullet's Y position
	const float& GetPosY();
	
	
	// Set the float representing the bullet's X position
	void SetPosX(float position);
	
	// Set the float representing the bullet's Y position
	void SetPosY(float position);
	
	
	// Returns the bool representing the bullet's alive variable
	const bool& IsAlive();
	
	// Set the bool representing the bullet's alive variable
	void SetAlive(bool alive);
};

#endif


