#include "BSpline.h"

BSpline::BSpline()
{
	AddComponent<PositionComponent>(0.0f, 0.0f, 0.0f);
	AddComponent<RenderComponent>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "bSpline");
	
}

BSpline::~BSpline()
{

}
