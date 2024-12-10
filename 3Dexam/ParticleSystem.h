#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstdlib> // For rand
#include <cmath>   // For sin, cos
#include "Component.h" // Assuming ParticleComponent is in a separate header

class ParticleSystem {
public:
    // Emit a new particle
    void emit(ParticleComponent& particles, const glm::vec3& centerPosition,
        float minSpeed = 5.0f, float maxSpeed = 10.0f,
        float spreadX = 80.0f, float spreadZ = 80.0f) {
        if (particles.count < MAX_PARTICLES) {
            size_t index = particles.count; // Use the next available slot

            // Random position spread on X and Z axes around the center
            float offsetX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadX;
            float offsetZ = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadZ;
            glm::vec3 startPosition = centerPosition + glm::vec3(offsetX, 0.0f, offsetZ);

            // Random downward velocity with slight X and Z variations
            float speed = minSpeed + static_cast<float>(rand()) / RAND_MAX * (maxSpeed - minSpeed);
            float velocityX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.5f; // Small side drift
            float velocityZ = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.5f; // Small side drift
            glm::vec3 velocity = glm::vec3(velocityX, -speed, velocityZ); // Downward movement

            // Set particle properties
            particles.positions[index] = startPosition;
            particles.velocities[index] = velocity;
            particles.colors[index] = glm::vec4(1.0, 1.0, 1.0, 0.0);
           
            particles.lifetimes[index] = 4.0f; // Lifetime of 2 seconds

            ++particles.count;
        }
    }

    // Update all particles
    void update(ParticleComponent& particles, float deltaTime) {
        for (size_t i = 0; i < particles.count; ++i) {
            // Update position
            particles.positions[i] += particles.velocities[i] * deltaTime;

            // Decrease lifetime
            particles.lifetimes[i] -= deltaTime;

            // Fade out based on remaining lifetime
            if (particles.lifetimes[i] > 0) {
                particles.colors[i].a = particles.lifetimes[i] / 2.0f; // Normalize alpha to lifetime
            }

            // Remove dead particles
            if (particles.lifetimes[i] <= 0) {
                --particles.count;
                if (i != particles.count) { // Swap with the last active particle
                    particles.positions[i] = particles.positions[particles.count];
                    particles.velocities[i] = particles.velocities[particles.count];
                    particles.colors[i] = particles.colors[particles.count];
                    particles.lifetimes[i] = particles.lifetimes[particles.count];
                }
            }
        }
    }

    
};
