#pragma once

#include "Feature.h"

class ProjectedAreaFeature : public Feature<float>
{
public:
	ProjectedAreaFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

