#include "CollisionSystem.h"
#include "QuadTree.h"

void CollisionSystem::AABBCollision(Entity& entityA, std::vector<Entity> entities)
{
    auto* collisionComponentA = entityA.GetComponent<CollisionComponent>();
    auto* positionComponentA = entityA.GetComponent<CollisionComponent>();

    if (collisionComponentA && positionComponentA) {

    }
}

void CollisionSystem::BarycentricCoordinates(Entity& ballEntity, Entity& planeEntity, const std::shared_ptr<PhysicsSystem>& physicsSystem)
{
    // Accessing relevant components from ballEntity and planeEntity
    auto* positionComponent = ballEntity.GetComponent<PositionComponent>();
    auto* velocityComponent = ballEntity.GetComponent<VelocityComponent>();
    auto* ballRenderComponent = ballEntity.GetComponent<RenderComponent>();
    auto* planePositionComponent = planeEntity.GetComponent<PositionComponent>();
    auto* planeRenderComponent = planeEntity.GetComponent<RenderComponent>();
    if (ballEntity.isMarkedForDeletion) return;
    if (!positionComponent || !velocityComponent || !ballRenderComponent || !planePositionComponent || !planeRenderComponent) {
        return; // Ensuring all components exist
    }

    glm::vec3 point = positionComponent->position;
    glm::vec3 ballSize = ballRenderComponent->size;
    std::vector<Vertex>& planeVertices = planeRenderComponent->vertices;
    double groundThreshold = ballSize.y;

    // If the plane has no vertices, return early
    if (planeVertices.empty()) {
        return;
    }

    for (int i = 0; i < planeRenderComponent->Draw.GetIndices().size(); i += 3) {
        int index0 = planeRenderComponent->Draw.GetIndices()[i];
        int index1 = planeRenderComponent->Draw.GetIndices()[i + 1];
        int index2 = planeRenderComponent->Draw.GetIndices()[i + 2];

        glm::vec3 v0 = glm::vec3(
            (planeVertices[index0].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
            (planeVertices[index0].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
            (planeVertices[index0].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

        glm::vec3 v1 = glm::vec3(
            (planeVertices[index1].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
            (planeVertices[index1].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
            (planeVertices[index1].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

        glm::vec3 v2 = glm::vec3(
            (planeVertices[index2].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
            (planeVertices[index2].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
            (planeVertices[index2].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

        glm::vec3 v0v1 = v1 - v0;
        glm::vec3 v0v2 = v2 - v0;
        glm::vec3 v0p = point - v0;

        // Computing dot products
        double dot00 = glm::dot(v0v1, v0v1);
        double dot01 = glm::dot(v0v1, v0v2);
        double dot02 = glm::dot(v0v1, v0p);
        double dot11 = glm::dot(v0v2, v0v2);
        double dot12 = glm::dot(v0v2, v0p);

        // Computing barycentric coordinates
        double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
        double v = (dot11 * dot02 - dot01 * dot12) * invDenom;
        double w = (dot00 * dot12 - dot01 * dot02) * invDenom;
        double u = 1 - v - w;

        // If the point is inside the triangle (u, v, w > 0)
        if (u >= 0 && v >= 0 && w >= 0) {
            double height = v0.y * u + v1.y * v + v2.y * w;
            // Adjusting position and velocity if ball is near the ground
            glm::vec3 currentVelocity = velocityComponent->velocity;
            float frictionCoefficient = (planeVertices[index0].friction +
                planeVertices[index1].friction +
                planeVertices[index2].friction) / 3.0f;
           

            if (positionComponent->position.y < height + groundThreshold) {
                // Stopping downward motion and applying corrective force
                if (currentVelocity.y < 0) {
                    currentVelocity.y = 0.0f; // Stopping downward motion
                }

                // Applying upward force to counteract gravity
                physicsSystem->ApplyForce(ballEntity, glm::vec3(0.0f, 9.81f, 0.0f));
                velocityComponent->velocity = currentVelocity;

                // Applying corrective force if sinking
                if (positionComponent->position.y < height + groundThreshold)
                {
                    positionComponent->position.y = height + groundThreshold;
                }

                // Calculating the normal vector for the slope
                glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
                if (glm::length(normal) == 0.0f) continue; // Skipping degenerate triangles

                float inclineAngle = std::acos(normal.y);
                glm::vec3 slopeVector = glm::normalize(glm::vec3(normal.x, 0.0f, normal.z)); // Slope direction

                
               

                velocityComponent->velocity = currentVelocity;
               

                //std::cout << "Friction: " << frictionForce.x << ", " << frictionForce.y << ", " << frictionForce.z << std::endl;

                // Calculating gravity effect along the slope and apply force
               
                    glm::vec3 gravityAlongSlope = physicsSystem->CalculateGravity(inclineAngle, slopeVector, normal, frictionCoefficient);
                    if (glm::length(currentVelocity) > 0.0f) {
                        glm::vec3 velocityDirection = glm::normalize(currentVelocity);
                        glm::vec3 frictionForce = -frictionCoefficient * glm::length(normal) * velocityDirection;

                        // Cap the friction force to not exceed the current velocity's magnitude
                        if (glm::length(frictionForce) > glm::length(currentVelocity)) {
                            frictionForce = -currentVelocity; // Fully opposes motion, stopping the object
                        }

                        // Update velocity considering both gravity and friction
                        //velocityComponent->velocity = currentVelocity + gravityAlongSlope;
                    }
                    else {
                        // If velocity is zero, only gravity acts
                        //velocityComponent->velocity += gravityAlongSlope;
                    }
                    physicsSystem->ApplyForce(ballEntity, gravityAlongSlope);
                

            }

            return; // Exiting after processing the first intersecting triangle
        }
    }
}

void CollisionSystem::DODBarycentric(PositionStorage& storage, AccelerationStorage& aStorage, VelocityStorage& vStorage, std::vector<Entity*>& entityList, Entity& planeEntity, const std::shared_ptr<PhysicsSystem>& physicsSystem)
{

    for (auto* entity : entityList) {
        int entityID = entity->GetId();  // Assuming each entity has an ID
        auto* positionComponent = entityList[entityID]->GetComponent<PositionComponent>();
        auto* velocityComponent = entityList[entityID]->GetComponent<VelocityComponent>();
        auto* ballRenderComponent = entityList[entityID]->GetComponent<RenderComponent>();
        auto* planePositionComponent = planeEntity.GetComponent<PositionComponent>();
        auto* planeRenderComponent = planeEntity.GetComponent<RenderComponent>();
        if (entityList[entityID]->isMarkedForDeletion) return;
        if (!positionComponent || !velocityComponent || !ballRenderComponent || !planePositionComponent || !planeRenderComponent) {
            return; // Ensuring all components exist
        }

        glm::vec3 point = positionComponent->position;
        glm::vec3 ballSize = ballRenderComponent->size;
        std::vector<Vertex>& planeVertices = planeRenderComponent->vertices;
        double groundThreshold = ballSize.y;

        // If the plane has no vertices, return early
        if (planeVertices.empty()) {
            return;
        }

        for (int i = 0; i < planeRenderComponent->Draw.GetIndices().size(); i += 3) {
            int index0 = planeRenderComponent->Draw.GetIndices()[i];
            int index1 = planeRenderComponent->Draw.GetIndices()[i + 1];
            int index2 = planeRenderComponent->Draw.GetIndices()[i + 2];

            glm::vec3 v0 = glm::vec3(
                (planeVertices[index0].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
                (planeVertices[index0].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
                (planeVertices[index0].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

            glm::vec3 v1 = glm::vec3(
                (planeVertices[index1].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
                (planeVertices[index1].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
                (planeVertices[index1].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

            glm::vec3 v2 = glm::vec3(
                (planeVertices[index2].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
                (planeVertices[index2].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
                (planeVertices[index2].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

            glm::vec3 v0v1 = v1 - v0;
            glm::vec3 v0v2 = v2 - v0;
            glm::vec3 v0p = point - v0;

            // Computing dot products
            double dot00 = glm::dot(v0v1, v0v1);
            double dot01 = glm::dot(v0v1, v0v2);
            double dot02 = glm::dot(v0v1, v0p);
            double dot11 = glm::dot(v0v2, v0v2);
            double dot12 = glm::dot(v0v2, v0p);

            // Computing barycentric coordinates
            double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
            double v = (dot11 * dot02 - dot01 * dot12) * invDenom;
            double w = (dot00 * dot12 - dot01 * dot02) * invDenom;
            double u = 1 - v - w;

            // If the point is inside the triangle (u, v, w > 0)
            if (u >= 0 && v >= 0 && w >= 0) {
                double height = v0.y * u + v1.y * v + v2.y * w;
                // Adjusting position and velocity if ball is near the ground
                glm::vec3 currentVelocity = velocityComponent->velocity;



                if (positionComponent->position.y < height + groundThreshold) {
                    // Stopping downward motion and applying corrective force
                    if (currentVelocity.y < 0) {
                        currentVelocity.y = 0.0f; // Stopping downward motion
                    }

                    // Applying upward force to counteract gravity
                    physicsSystem->ApplyForce(*entityList[entityID], glm::vec3(0.0f, 9.81f, 0.0f));
                    velocityComponent->velocity = currentVelocity;

                    // Applying corrective force if sinking
                    if (positionComponent->position.y < height + groundThreshold)
                    {
                        positionComponent->position.y = height + groundThreshold;
                    }

                    // Calculating the normal vector for the slope
                    glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
                    if (glm::length(normal) == 0.0f) continue; // Skipping degenerate triangles

                    float inclineAngle = std::acos(normal.y);
                    glm::vec3 slopeVector = glm::normalize(glm::vec3(normal.x, 0.0f, normal.z)); // Slope direction

                    // Adjusting velocity based on slope incline
                    float speedAdjustment = glm::dot(currentVelocity, slopeVector);
                    if (currentVelocity.y > 0) { // Ball is moving upward
                        currentVelocity.y -= speedAdjustment * sin(inclineAngle);

                        // Ensuring ball doesn't go through the floor
                        if (positionComponent->position.y < height + groundThreshold) {
                            positionComponent->position.y = height + groundThreshold;
                            currentVelocity.y = 0; // Stopping upward motion
                        }
                    }
                    else if (currentVelocity.y < 0) { // Ball is moving downward
                        currentVelocity.y += speedAdjustment * sin(inclineAngle);

                        // Ensuring ball doesn't go through the floor
                        if (positionComponent->position.y < height + groundThreshold) {
                            positionComponent->position.y = height + groundThreshold;

                            currentVelocity.y = 0; // Stopping downward motion
                        }
                    }

                    velocityComponent->velocity = currentVelocity;


                    // Calculating gravity effect along the slope and apply force
                    if (glm::length(slopeVector) > 0.00000001f) {
                        glm::vec3 gravityAlongSlope = physicsSystem->CalculateGravity(inclineAngle, slopeVector, normal, 1.0f);
                        physicsSystem->ApplyForce(*entityList[entityID], gravityAlongSlope);
                    }

                }

                return; // Exiting after processing the first intersecting triangle
            }
        }
       
    }

}

void CollisionSystem::UpdateCollision(QuadTree* quadTree, std::vector<Entity*>& allEntities, float dt)
{
    //std::vector<Entity*> allEntities; // Assume you have a method to populate this
    quadTree->ClearTree();

    // Insert all entities into the quadtree
    for (Entity* entity : allEntities) {
        quadTree->Insert(entity);
    }

    // Check collisions using the quadtree
    for (Entity* entity : allEntities) {
        std::vector<Entity*> potentialColliders;
        quadTree->Retrieve(potentialColliders, entity); // Get nearby entities
        CheckCollision(entity, potentialColliders, 0, dt); // Check for collisions
    }
}

void CollisionSystem::CheckCollision(Entity* object, std::vector<Entity*>& potentialColliders, int startingIndex, float dt)
{
    for (size_t i = 0; i < potentialColliders.size(); ++i)
    {
        if (object != potentialColliders[i]) { // Avoid self-collision
            SphereCollision(*object, *potentialColliders[i], dt);
        }
    }
}

void CollisionSystem::ObjectCollisionResponse(Entity& objA, Entity& objB)
{
    float massA = objA.GetComponent<PhysicsComponet>()->mass;
    //needs to add mass to the object
    float massB = objB.GetComponent<PhysicsComponet>()->mass;
    glm::vec3 posA = objA.GetComponent<PositionComponent>()->position;
    glm::vec3 posB = objB.GetComponent<PositionComponent>()->position;
    glm::vec3 velocityA = objA.GetComponent<VelocityComponent>()->velocity;
    glm::vec3 velocityB = objB.GetComponent<VelocityComponent>()->velocity;

    glm::vec3 normal = glm::normalize(posB - posA);					// Calculating the normal vector of the collision
    glm::vec3 relativeVelocity = velocityA - velocityB;				// Calculating the relative velocity
    float velocityAlongNormal = glm::dot(relativeVelocity, normal);	// Calculating the velocity component along the normal

    // Calculating the new velocities along the normal direction
    float restitution = 0.01f; // Coefficient of restitution (1 = perfectly elastic collision)
    float impulse = (-(1 + restitution) * velocityAlongNormal) / (1 / massA + 1 / massB);

    glm::vec3 impulseVector = impulse * normal;

    // Updating velocities along the normal
    glm::vec3 newVelocityA = velocityA + (impulseVector / massA);
    glm::vec3 newVelocityB = velocityB - (impulseVector / massB);

    // Setting new velocities
    objA.GetComponent<VelocityComponent>()->velocity = newVelocityA;
    objB.GetComponent<VelocityComponent>()->velocity = newVelocityB;
}

bool CollisionSystem::InvAABBCollision(Entity& entityA, Entity& entityB, float deltaTime)
{
    if (entityA.isMarkedForDeletion || entityB.isMarkedForDeletion) return false;
    auto* positionA = entityA.GetComponent<PositionComponent>();
    auto* velocityB = entityB.GetComponent<VelocityComponent>();
    auto* positionB = entityB.GetComponent<PositionComponent>();
    glm::vec3 sizeA = entityA.GetComponent<RenderComponent>()->size;
    glm::vec3 sizeB = entityB.GetComponent<RenderComponent>()->size;
    // Ensuring all required components are present


    // Extracting the velocity of entity B
    glm::vec3 velocity = velocityB->velocity;

    // Calculating the new position of entity B based on its velocity and deltaTime
    glm::vec3 newPosB = positionB->position + velocity * deltaTime;

    // Checking collision on the X-axis
    if (abs(newPosB.x - positionA->position.x) > (sizeA.x - sizeB.x)) {
        // Inverting X velocity
        velocity.x *= -1.0f;
    }

    // Checking collision on the Z-axis
    if (abs(newPosB.z - positionA->position.z) > (sizeA.z - sizeB.z)) {
        // Inverting Z velocity
        velocity.z *= -1.0f;
    }

    // Updating velocity component of entity B
    velocityB->velocity = velocity;

    // Updating angular velocity if the component exists
    auto* angularVelocityB = entityB.GetComponent<AngularVelocityComponent>();
    if (angularVelocityB) {
        angularVelocityB->angularvelocity = velocity;
    }

    return true;
}

bool CollisionSystem::SphereCollision(Entity& objA, Entity& objB, float deltaTime)
{
    //float VelocityScale = 0.05f;
    glm::vec3 posA = objA.GetComponent<PositionComponent>()->GetPosition() + objA.GetComponent<VelocityComponent>()->GetVelocity() * deltaTime;
    glm::vec3 posB = objB.GetComponent<PositionComponent>()->GetPosition() + objB.GetComponent<VelocityComponent>()->GetVelocity() * deltaTime;
    float distance_centers = glm::length(posA - posB);
    float combinedRadii = objA.GetComponent<RenderComponent>()->size.x + objB.GetComponent<RenderComponent>()->size.x;
    float offset = 0.1f;
    std::cout << "collition" << std::endl;
    if (distance_centers <= (combinedRadii + offset)) {

        // Calculate the minimum translation distance to resolve the collision
        float minimuntranslation = (combinedRadii + offset) - distance_centers;

        // Determine the direction vector between the two objects
        auto dirvec = glm::normalize(posA - posB);

        // Apply the translation to separate objA from objB
        objA.GetComponent<PositionComponent>()->position += dirvec * minimuntranslation;

        // Handle collision response
        ObjectCollisionResponse(objA, objB);

        return true;
    }

    // No collision detected
    return false;
}
