#include "BallObject.h"
#include <cstdlib> 
#include <ctime>  
#include <random> // For random seeds

BallObject::BallObject()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> dis(0.0, 1.0);

	// Generate random color values
	float r = dis(gen);
	float g = dis(gen);
	float b = dis(gen);

	AddComponent<PositionComponent>(0, 0.0f,0.0);
	AddComponent<VelocityComponent>();
	AddComponent<AccelerationComponent>();
	AddComponent<RenderComponent>(glm::vec3(r,g,b), glm::vec3(1.0f, 1.0f, 1.0f), "sphere");
	AddComponent<PhysicsComponet>();
	
}

BallObject::~BallObject()
{
	// Destructor

}
