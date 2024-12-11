#include "QuadTree.h"
#include <glm/glm.hpp>

QuadTree::QuadTree(int depth, glm::vec4 bounds) : depth(depth), bounds(bounds)
{
	for (int i = 0; i < 4; ++i) {

		nodes[i] = nullptr;
	}
}

void QuadTree::ClearTree()
{
	// clear balls 
	balls.clear();
	//clear tree of nodes
	for (int i = 0; i < 4; ++i) {

		if (nodes[i] != nullptr) {
			nodes[i]->ClearTree();
			delete nodes[i];
			nodes[i] = nullptr;

		}
	}

}

void QuadTree::SplitTree()
{
	float subWidth = bounds.z / 2.0f;
	float subHeight = bounds.w / 2.0f;
	float x = bounds.x;
	float y = bounds.y;

	nodes[0] = new QuadTree(depth + 1, glm::vec4(x + subWidth, y, subWidth, subHeight)); // Top-right quadrant
	nodes[1] = new QuadTree(depth + 1, glm::vec4(x, y, subWidth, subHeight));            // Top-left quadrant
	nodes[2] = new QuadTree(depth + 1, glm::vec4(x, y + subHeight, subWidth, subHeight)); // Bottom-left quadrant
	nodes[3] = new QuadTree(depth + 1, glm::vec4(x + subWidth, y + subHeight, subWidth, subHeight)); // Bottom-right quadrant
}

void QuadTree::Insert(Entity* obj)
{
	if (nodes[0] != nullptr) {
		int index = GetIndex(obj);
		if (index != -1) {
			nodes[index]->Insert(obj);
			return;
		}
	}

	balls.push_back(obj);
	if (balls.size() > maxDepth && depth < maxDepth) {
		if (nodes[0] == nullptr) {
			SplitTree();

		}

		int i = 0;
		while (i < balls.size()) {
			int index = GetIndex(balls[i]);
			if (index != -1) {
				nodes[index]->Insert(balls[i]);
				balls.erase(balls.begin() + i);
			}
			else {
				++i;
			}
		}
	}

}

int QuadTree::GetIndex(Entity* obj)
{
	int index = -1;
	float verticalMidpoint = bounds.x + (bounds.z / 2.0f);
	float horizontalMidpoint = bounds.y + (bounds.w / 2.0f);

	// Object dimensions
	glm::vec3 objPos = obj->GetComponent<PositionComponent>()->GetPosition();
	glm::vec3 objSize = obj->GetComponent<RenderComponent>()->size;

	bool topQuadrant = (objPos.y < horizontalMidpoint && objPos.y + objSize.y < horizontalMidpoint);
	bool bottomQuadrant = (objPos.y > horizontalMidpoint);

	if (objPos.x < verticalMidpoint && objPos.x + objSize.x < verticalMidpoint) {
		if (topQuadrant) {
			index = 1;  // Top-left quadrant
		}
		else if (bottomQuadrant) {
			index = 2;  // Bottom-left quadrant
		}
	}
	else if (objPos.x > verticalMidpoint) {
		if (topQuadrant) {
			index = 0;  // Top-right quadrant
		}
		else if (bottomQuadrant) {
			index = 3;  // Bottom-right quadrant
		}
	}
	return index;
}

std::vector<Entity*> QuadTree::Retrieve(std::vector<Entity*>& returnObjects, Entity* obj)
{
	int index = GetIndex(obj);
	if (index != -1 && nodes[0] != nullptr) {
		nodes[index]->Retrieve(returnObjects, obj);
	}

	returnObjects.insert(returnObjects.end(), balls.begin(), balls.end());

	return returnObjects;
}
