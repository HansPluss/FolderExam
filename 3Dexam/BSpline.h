#pragma once
#include "Entity.h"





class BSpline : public Entity
{
public:
	BSpline();
	~BSpline();


	Entity* attachedBall = nullptr;


};