#pragma once
#include "Feature.h"

class DiscontinuityFeature : public Feature<std::vector<float>>
{
public:
	DiscontinuityFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

