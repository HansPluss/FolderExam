#include "Player.h"
#include <memory>
Player::Player()
{
	AddComponent<PositionComponent>(10.0f, 0.0f, 0.0f);
	AddComponent<VelocityComponent>();
	AddComponent<AccelerationComponent>();
	AddComponent<InputComponent>();
	AddComponent<RenderComponent>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "sphere");
	AddComponent<PhysicsComponet>();
	AddComponent<HealthComponent>(100);
	

	
}


Player::~Player()
{
	// Destructor
}

