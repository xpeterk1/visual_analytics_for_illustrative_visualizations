#pragma once
#include "Feature.h"
class ViewDependentCurvatureFeature : public Feature<std::vector<glm::vec3>>
{
public:
	ViewDependentCurvatureFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};