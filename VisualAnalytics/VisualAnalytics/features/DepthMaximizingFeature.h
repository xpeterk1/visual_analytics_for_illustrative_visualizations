#pragma once
#include "Feature.h"
#include <stdlib.h>

class DepthMaximizingFeature : public Feature<float>
{
public:
	DepthMaximizingFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};
