#pragma once

#include "Feature.h"

class NormalFeature : public Feature<std::vector<glm::vec3>>
{
public:
	NormalFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

