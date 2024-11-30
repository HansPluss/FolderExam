#pragma once
#include "Entity.h"





class BallObject : public Entity
{
public:
	BallObject();
	~BallObject();
	
	Entity* attachedSpline = nullptr;
	
	
};

