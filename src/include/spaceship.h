/*
 *  Spaceship.h
 *  
 *  Created by LoGi on 02/07/2015
 *  Copyright 2015 All rights reserved
 */

#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <nds.h> 		// Includes libnds
#include <nf_lib.h> 	// Includes NightFox Lib

using namespace std;

class Spaceship {

protected:

	float _position_X;
	float _position_Y;
    float _speed;
	int _score;
	int8 _lives;
	u8 _width;
	u8 _height;
	u8 _animationTimer;
	u8 _frame;
 
public:
	 
    // Spaceship constructor
    Spaceship();

    // Spaceship destructor
    virtual ~Spaceship();
	 
    // Moves the spaceship right
	void MoveRight();

    // Moves the spaceship left
	void MoveLeft();

	void Load();

	void Update();
	
	void Draw();
	
	void Hit();
	
	void Reset();

    // Returns the float representing the spaceship's X position
	const float& GetPosX();
	
	// Returns the float representing the spaceship's Y position
	const float& GetPosY();

	// Returns the u8 representing the spaceship's current frame number
	const u8& GetFrame();

	// Returns the int8 representing the players lives
	const int8& GetPlayerLives();

	// Set the int8 representing the players score
	void SetPlayerLives(int8 playerLives);

	// Returns the int representing the players score
	const int& GetPlayerScore();
	
	// Set the int representing the players score
	void SetPlayerScore(int playerScore);
};

#endif
