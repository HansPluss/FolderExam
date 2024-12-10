#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <array>
#include "draw.h"
#include <iostream>
extern "C" {
#include "lua54/include/lua.h"
#include "lua54/include/lauxlib.h"
#include "lua54/include/lualib.h"

}

#ifdef _WIN32
#pragma comment(lib, "lua54/lua54.lib")
#endif
// TO DO ADD COMMENTS
class Component {
public:
    virtual ~Component() = default; // default destructor
};

class PositionComponent : public Component {
public:
    glm::vec3 position; // position for the object

    PositionComponent(float x = 0, float y = 0, float z = 0)
        : position(x, y, z) {}
    glm::vec3 GetPosition() {

        return position;

    }
    void SetPosition(glm::vec3 newPosition) {
        position = newPosition;

    }
};
class VelocityComponent : public Component {
public:
    glm::vec3 velocity; // velocity for the object, used for movement

    VelocityComponent(float vx = 0, float vy = 0, float vz = 0)
        : velocity(vx, vy, vz) {}
    glm::vec3 GetVelocity() {

        return velocity;

    }
    void SetVelocity(glm::vec3 newVelocity) {
        velocity = newVelocity;

    }
};
class AngularVelocityComponent : public Component {
public:
    glm::vec3 angularvelocity; // velocity for the object, used for movement

    AngularVelocityComponent(float avx = 0, float avy = 0, float avz = 0)
        : angularvelocity(avx, avy, avz) {}
    glm::vec3 GetAngularVelocity() {

        return angularvelocity;

    }
    void SetAnuglarVelocity(glm::vec3 newVelocity) {
        angularvelocity = newVelocity;

    }
};
class AccelerationComponent : public Component {
public:
    glm::vec3 acceleration; // acceleration for the object, used for movement

    AccelerationComponent(float ax = 0, float ay = 0, float az = 0)
        : acceleration(ax, ay, az) {}
};
class HealthComponent : public Component {
public:
    float health; // health for the combat system
    HealthComponent(float hp = 100)
        : health(hp) {}

};
class DamageComponent : public Component {
public:
    float damage; // damage for the combat system
    DamageComponent(float dmg = 10.0f)
        : damage(dmg) {}

};
class RenderComponent : public Component {
public:
    //Handles all of the variables for rendering an object
    glm::vec3 color;
    glm::vec3 size;
    glm::vec3 rotation;
    std::string shape; // Could be "cube", "plane", "sphere", "etc".
    std::vector<Vertex> vertices;
    Draw Draw; // class that houses all the rendering functions
    RenderComponent(const glm::vec3& color, const glm::vec3& size, const std::string& shape, const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f))
        : color(color), size(size), shape(shape), rotation(rotation) {}
};
class CollisionComponent : public Component {
public:
    glm::vec3 size; // Can be added for custom collision box/sphere
    float radius;
    bool isColliding = false;
    CollisionComponent(float sizex = 1.0f, float sizey = 1.0f, float sizez = 1.0f, float radius = 10.0f)
        : size(glm::vec3(sizex, sizey, sizez)), radius(radius) {


    }

};
class PhysicsComponet : public Component {
public:
    float mass; // For physics objects
    float gravity;
    PhysicsComponet(float mass = 1.0f, float gravity = 9.81f) :
        mass(mass), gravity(gravity) {


    }


};
class AIComponent : public Component {
public:
    float speed;
    float detectionRadius;
    lua_State* luaState;        // Lua state for this component

    AIComponent(float speed = 2.0f, float radius = 50.0f)
        : speed(speed), detectionRadius(radius), luaState(luaL_newstate()) {
        if (!luaState) {
           // throw std::runtime_error("Failed to create Lua state");
        }
        luaL_openlibs(luaState); // Load Lua standard libraries
    }

    ~AIComponent() {
        if (luaState) {
            lua_close(luaState); // Close the Lua state when done
        }
    }

    void LoadScript(const std::string& scriptPath) {
        if (luaL_dofile(luaState, scriptPath.c_str()) != LUA_OK) {
            std::cout << "Error loading Lua script: " << lua_tostring(luaState, -1) << std::endl;
            std::cout << "hello";
            lua_pop(luaState, 1); // Remove error message from the stack
        }
        else {
            std::cout << "SUCCESS" << std::endl;
        }


    }

    void UpdateAI() {
        lua_getglobal(luaState, "AI_Behavior");
       
        if (!lua_isfunction(luaState, -1)) {
            std::cout << "AI_Behavior function not found in Lua script!" << std::endl;
            lua_pop(luaState, 1);
            return;
        }
        
        lua_pushnumber(luaState, speed);
        lua_pushnumber(luaState, detectionRadius);

        if (lua_pcall(luaState, 2, 1, 0) != LUA_OK) { // Expect 1 return value
            std::cout << "Error executing AI_Behavior: " << lua_tostring(luaState, -1) << std::endl;
            lua_pop(luaState, 1);
            return;
        }
      
        // Retrieve the return value
        if (lua_isnumber(luaState, -1)) {
            speed = static_cast<float>(lua_tonumber(luaState, -1));
        }
        lua_pop(luaState, 1); // Clean up the stack
        ReloadScript("aiBehavoir.lua"); // Reloads the script
    }
    void ReloadScript(const std::string& scriptPath) {
        // Close any existing Lua state before reloading the script
        lua_close(luaState);
        luaState = luaL_newstate();
        luaL_openlibs(luaState);

        // Reload and run the Lua script
        if (luaL_dofile(luaState, scriptPath.c_str()) != LUA_OK) {
            std::cerr << "Error running Lua script: " << lua_tostring(luaState, -1) << std::endl;
            lua_pop(luaState, 1);
        }

        // Get the AI_Behavior function again
        lua_getglobal(luaState, "AI_Behavior");
        if (!lua_isfunction(luaState, -1)) {
            std::cerr << "Error: AI_Behavior function not found in Lua script!" << std::endl;
            lua_pop(luaState, 1);
        }
    }
};
const size_t MAX_PARTICLES = 5000;
class ParticleComponent : public Component {
public:
    // Particle data arrays (Structure of Arrays)
    std::array<glm::vec3, MAX_PARTICLES> positions;   // Particle positions
    std::array<glm::vec3, MAX_PARTICLES> velocities;  // Particle velocities
    std::array<glm::vec4, MAX_PARTICLES> colors;      // Particle colors (RGBA)
    std::array<float, MAX_PARTICLES> lifetimes;       // Particle lifetimes

    size_t count; // Current number of active particles
    Draw draw;
    // Default constructor
    ParticleComponent()
        : count(0) { // Initialize count to zero
        // Initialize all particles to default values
        for (size_t i = 0; i < MAX_PARTICLES; ++i) {
            positions[i] = glm::vec3(0.0f);            // Default position at origin
            velocities[i] = glm::vec3(0.0f);           // No initial velocity
            colors[i] = glm::vec4(1.0f, 1.0f, 1.0f, 0.50f); // Fully transparent white
            lifetimes[i] = 0.0f;                       // Zero lifetime (inactive)
        }
    }



};
class InputComponent : public Component {
public:
    // Stores all variables related to input, for example for the player class to use
    bool moveUp, moveDown, moveLeft, moveRight, bTab;
    bool bKey1, bKey2, bKey3, bKey4, bKey5, bKey6, bKey7, bKey8, bKey9, bKeyQ;

    InputComponent()
        : moveUp(false), moveDown(false), moveLeft(false), moveRight(false),
        bTab(false), bKey1(false), bKey2(false), bKey3(false), bKey4(false),
        bKey5(false), bKey6(false), bKey7(false), bKey8(false),
        bKey9(false), bKeyQ(false) {}

    // Updating input states
    void updateInput(bool up, bool down, bool left, bool right, bool Tab,
        bool key1, bool key2, bool key3, bool key4,
        bool key5, bool key6, bool key7, bool key8,
        bool key9, bool keyQ) {
        moveUp = up;
        moveDown = down;
        moveLeft = left;
        moveRight = right;
        bTab = Tab;

        // Updating number key states
        bKey1 = key1;
        bKey2 = key2;
        bKey3 = key3;
        bKey4 = key4;
        bKey5 = key5;
        bKey6 = key6;
        bKey7 = key7;
        bKey8 = key8;
        bKey9 = key9;
        bKeyQ = keyQ;
    }
};
class PositionStorage {
public:
    // For DOD purposes, stores all position componenets in a vector for batch processing
    std::vector<glm::vec3> positions;
    std::vector<int> entityIDs;

    void AddPosition(const glm::vec3& pos, int entityID) {
        positions.push_back(pos);
        entityIDs.push_back(entityID);
    }

    glm::vec3& GetPositionByEntityID(int entityID) {
        auto it = std::find(entityIDs.begin(), entityIDs.end(), entityID);
        if (it != entityIDs.end()) {
            return positions[std::distance(entityIDs.begin(), it)];
        }

    }
    void RemovePositionByEntityID(int entityID) {
        auto it = std::find(entityIDs.begin(), entityIDs.end(), entityID);
        if (it != entityIDs.end()) {
            // Find the index of the entityID
            size_t index = std::distance(entityIDs.begin(), it);

            // Remove the position and entityID at the found index
            positions.erase(positions.begin() + index);
            entityIDs.erase(entityIDs.begin() + index);
        }
    }
    bool HasEntity(int entityID) {
        for (auto id : entityIDs) {
            if (id == entityID) {
                return true;
            }

        }
        return false;
    }
};

class VelocityStorage {
public:
    // For DOD purposes, stores all velocity componenets in a vector for batch processing
    std::vector<glm::vec3> velocities;
    std::vector<int> entityIDs;

    void AddVelocity(const glm::vec3& vel, int entityID) {
        velocities.push_back(vel);
        entityIDs.push_back(entityID);
    }

    glm::vec3& GetVelocityByEntityID(int entityID) {
        auto it = std::find(entityIDs.begin(), entityIDs.end(), entityID);
        if (it != entityIDs.end()) {
            return velocities[std::distance(entityIDs.begin(), it)];
        }


    }
    void RemoveVelocityByEntityID(int entityID) {
        auto it = std::find(entityIDs.begin(), entityIDs.end(), entityID);
        if (it != entityIDs.end()) {
            // Find the index of the entityID
            size_t index = std::distance(entityIDs.begin(), it);

            // Remove the position and entityID at the found index
            velocities.erase(velocities.begin() + index);
            entityIDs.erase(entityIDs.begin() + index);
        }
    }
    bool HasEntity(int entityID) {
        return std::find(entityIDs.begin(), entityIDs.end(), entityID) != entityIDs.end();
    }
};

class AccelerationStorage {
public:
    // For DOD purposes, stores all acceleration componenets in a vector for batch processing
    std::vector<glm::vec3> accelerations;
    std::vector<int> entityIDs;

    void AddAcceleration(const glm::vec3& accel, int entityID) {
        accelerations.push_back(accel);
        entityIDs.push_back(entityID);
    }

    glm::vec3& GetAccelerationByEntityID(int entityID) {
        auto it = std::find(entityIDs.begin(), entityIDs.end(), entityID);
        if (it != entityIDs.end()) {
            return accelerations[std::distance(entityIDs.begin(), it)];
        }

    }
    void RemoveAccelerationByEntityID(int entityID) {
        auto it = std::find(entityIDs.begin(), entityIDs.end(), entityID);
        if (it != entityIDs.end()) {
            // Find the index of the entityID
            size_t index = std::distance(entityIDs.begin(), it);

            // Remove the position and entityID at the found index
            accelerations.erase(accelerations.begin(), accelerations.begin() + index);
            entityIDs.erase(entityIDs.begin(), entityIDs.begin() + index);
        }
    }
    bool HasEntity(int entityID) {
        return std::find(entityIDs.begin(), entityIDs.end(), entityID) != entityIDs.end();
    }
};
