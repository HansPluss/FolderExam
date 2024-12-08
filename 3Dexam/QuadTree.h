#pragma once
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Draw.h"
#include "Entity.h"
class QuadTree {
public:
	int maxObjects = 4;
	int maxDepth = 4;
	std::vector<Entity*> balls;
	int depth;
	glm::vec4 bounds;

	QuadTree* nodes[4];
	QuadTree(int depth, glm::vec4 bounds);
	void ClearTree();
	void SplitTree();
	void Insert(Entity* obj);
	int GetIndex(Entity* obj);

	std::vector<Entity*> Retrieve(std::vector<Entity*>& returnObjects, Entity* obj);
};