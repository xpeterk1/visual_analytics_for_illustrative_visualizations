#pragma once

#include "Feature.h"

class ContourDistanceFeature : public Feature<std::vector<float>>
{
public:
	ContourDistanceFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

