#include "Enemy.h"

Enemy::Enemy()
{
    AddComponent<PositionComponent>(0.0f, 10.0f, -10.0f);
    AddComponent<RenderComponent>(glm::vec3(0.0f), glm::vec3(1.0f), "cube");
    AddComponent<VelocityComponent>();
    AddComponent<AccelerationComponent>();
    AddComponent<AIComponent>();
    AddComponent<PhysicsComponet>(10);
    AddComponent<HealthComponent>(30);
    AddComponent<DamageComponent>(10);
    
    GetComponent<AIComponent>()->LoadScript("aiBehavoir.lua");
    //set lua script to AI component
}

void Enemy::FollowEntity(Entity& target, const std::shared_ptr<PhysicsSystem>& physicssystem)
{
    auto* followerPos = GetComponent<PositionComponent>();
    auto* velocity = GetComponent<VelocityComponent>();
    auto* targetPos = target.GetComponent<PositionComponent>();
    auto* ai = GetComponent<AIComponent>();

    ai->UpdateAI();

    if (followerPos && velocity && targetPos && ai) {
        glm::vec3 direction = targetPos->position - followerPos->position;

        // Computing the distance between entities
        float distance = glm::length(direction);
        playerref = &target;
        // Normalizing the direction vector to prevent zero-length vectors
        if (distance > 0.0001f) {
            glm::vec3 dirvec = glm::normalize(direction);

            // Applying force to move the follower toward the target
            glm::vec3 force = dirvec * ai->speed;

            // Updating the follower's velocity based on the force
            physicssystem->ApplyForce(*this, force);
        }
    }
}

void Enemy::Death(const std::shared_ptr<EntityManager>& manager, std::vector<Entity*>& entityList, const std::shared_ptr<RenderingSystem>& rendersystem, PositionStorage& storage, AccelerationStorage& aStorage, VelocityStorage& vStorage)
{

}
