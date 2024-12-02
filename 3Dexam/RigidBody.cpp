#include "RigidBody.h"
#include <iostream> // For std::cout

glm::vec3 RigidBody::CalculateGravity(float inclineAngle, glm::vec3 slopeVector, glm::vec3 normal, float friction)
{
    // Downward gravity force
    glm::vec3 gravityForce(0.0f, gravity, 0.0f);

    // Calculate the normal force (perpendicular to the slope)
    float normalForceMagnitude = glm::dot(gravityForce, normal); // Gravity along the normal
    glm::vec3 normalForce = normal * normalForceMagnitude;

    // Calculate the gravitational force acting parallel to the slope
    glm::vec3 gravityParallel = gravityForce - normalForce; // Force parallel to the slope

    // Project the parallel gravity onto the slope vector
    glm::vec3 gravityAlongSlope = glm::dot(gravityParallel, slopeVector) * slopeVector;

    // Calculate the friction force
    glm::vec3 frictionForce = glm::normalize(gravityAlongSlope) * -friction;

    // Clamp friction force to not exceed the gravity along the slope
    if (glm::length(frictionForce) > glm::length(gravityAlongSlope)) {
        frictionForce = -gravityAlongSlope; // Fully opposes the motion
    }

    // Apply the friction to the gravity along the slope
    glm::vec3 resultingForce = gravityAlongSlope + frictionForce;

    // Return the final force along the slope
    return resultingForce;
}




void RigidBody::ApplyForce(AccelerationComponent& aComponent,PhysicsComponet& physicsComponent, glm::vec3 force)
{
    aComponent.acceleration = force / physicsComponent.mass;
}

void RigidBody::Update(PositionComponent& pComponent, VelocityComponent& vComponent, AccelerationComponent& aComponent, float deltaTime)
{
    //updates the position of the entity
    //std::cout << pComponent.GetPosition().x << std::endl;
    vComponent.velocity += aComponent.acceleration * deltaTime;
    pComponent.position += vComponent.velocity * deltaTime;
    aComponent.acceleration = glm::vec3(0.0f, gravity, 0.0f);
}

void RigidBody::DODUpdate(PositionStorage& storage, AccelerationStorage& aStorage, VelocityStorage& vStorage, std::vector<Entity*>& entityList, float deltaTime) {
    const float timeScale = 0.5f; // Scale down time to slow things down
    const float damping = 0.98f;  // Damping to slow down velocities
    const float maxVelocity = 10.0f;  // Cap the velocity for stability

    // Adjust deltaTime for time scale
    float timeStep = deltaTime * timeScale;
    for (auto* entity : entityList) {
        int entityID = entity->GetId();



        if (entity->isMarkedForDeletion)
            return;
        // Ensure the entity has all the necessary components
        if (entity->GetComponent<PositionComponent>() && entity->GetComponent<VelocityComponent>() && entity->GetComponent<AccelerationComponent>()) {
            // Update velocity using acceleration
            glm::vec3& velocity = vStorage.GetVelocityByEntityID(entityID);
            glm::vec3& acceleration = aStorage.GetAccelerationByEntityID(entityID);

            velocity += acceleration * timeStep;



            // Update position using velocity
            glm::vec3& position = storage.GetPositionByEntityID(entityID);
            position += velocity * timeStep;

            // Apply gravity
            acceleration = glm::vec3(0.0f, gravity, 0.0f);

            // Apply friction (if on ground)


            // Update storage
            storage.GetPositionByEntityID(entityID) = position;
            aStorage.GetAccelerationByEntityID(entityID) = acceleration;
            vStorage.GetVelocityByEntityID(entityID) = velocity;

            // Update the entity's PositionComponent

            entity->GetComponent<PositionComponent>()->position = position;
            entity->GetComponent<AccelerationComponent>()->acceleration = acceleration;
            entity->GetComponent<VelocityComponent>()->velocity = velocity;
        }
    }
}

void RigidBody::DODApplyForce(AccelerationStorage& aStorage, glm::vec3 force, int Id)
{


    aStorage.GetAccelerationByEntityID(Id) += force;
    //std::cout << "Force: " << aStorage.GetAccelerationByEntityID(Id).x;
}


