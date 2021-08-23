/*
 *  Barrier.h
 *  
 *  Created by LoGi on 09/07/2015
 *  Copyright 2015 All rights reserved
 */

#ifndef Barrier_H
#define Barrier_H

#include <nds.h>		// Includes libnds
#include <nf_lib.h> 	// Includes NightFox Lib

using namespace std;


class Barrier {

protected:

	float _position_X;
	float _position_Y;
	u8 _id;
	u8 _width;
	u8 _height;
	u8 _frame;
	bool _alive;
	
public:
     
    // Barrier constructor
    Barrier(float x, float y, u8 id);

    // Barrier destructor
    virtual ~Barrier();
	 
	void Load();
	
	void Draw();
	 
	void Hit();
	
	void Reset();
	 
    // Returns the float representing the barrier's X position
	const float& GetPosX();
	
	// Returns the float representing the barrier's Y position
	const float& GetPosY();
	
	// Returns the bool representing the barrier's alive variable
	const bool& IsAlive();
};

#endif


