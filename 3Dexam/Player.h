#pragma once
#include "Entity.h"





class Player : public Entity
{
public: 
	Player();
	~Player();
	int GetMaxSpeed() { return maxSpeed; }
	int GetSpeed() { return speed; }
	void SetSpeed(int newSpeed) { speed = newSpeed; }
private:


	int maxSpeed = 20;
	int speed = 10; 
};

