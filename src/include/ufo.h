/*
 *  Ufo.h
 *  
 *  Created by LoGi on 09/07/2015
 *  Copyright 2015 All rights reserved
 */

#ifndef Ufo_H
#define Ufo_H

#include <nds.h>		// Includes libnds
#include <nf_lib.h> 	// Includes NightFox Lib

using namespace std;

class Ufo {

protected:

	float _position_X;
	float _position_Y;
	float _speed;
	u8 _id;
	u8 _width;
	u8 _height;
	u8 _frame;
	u8 _direction;
	u8 _animationTimer;
	bool _alive;
	bool _hit;
	
public:
     
    // Ufo constructor
    Ufo(float x, float y, u8 id);

    // Ufo destructor
    virtual ~Ufo();
	 
	void Load();
	
	void Draw();
	 
	void Update();
	 
	void Hit();
	 
    // Returns the float representing the Ufo's X position
	const float& GetPosX();
	
	// Returns the float representing the Ufo's Y position
	const float& GetPosY();
	
	
	// Set the float representing the Ufo's X position
	void SetPosX(float position);
	
	// Set the float representing the Ufo's Y position
	void SetPosY(float position);
	
	
	// Set the u8 representing the Ufo's direction
	void SetDirection(u8 direction);
	
	
	// Set the float representing the Ufo's speed
	//void SetSpeed(float speed);
	
	
	// Returns the bool representing the Ufo's alive variable
	const bool& IsAlive();
};

#endif