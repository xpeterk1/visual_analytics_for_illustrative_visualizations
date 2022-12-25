#pragma once

#include "Feature.h"
#include <unordered_set>

class VisibilityRatioFeature : public Feature<float>
{
public:
	VisibilityRatioFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

