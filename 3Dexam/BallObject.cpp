#include "BallObject.h"
#include <cstdlib> 
#include <ctime>  
BallObject::BallObject()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	float randomX = static_cast<float>(std::rand() % 1) - 1.0f;
	float randomZ = static_cast<float>(std::rand() % 1) - 1.0f;


	AddComponent<PositionComponent>(0, 0.0f,0.0);
	AddComponent<VelocityComponent>();
	AddComponent<AccelerationComponent>();
	AddComponent<RenderComponent>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "sphere");
	AddComponent<PhysicsComponet>();

}

BallObject::~BallObject()
{
	// Destructor

}
