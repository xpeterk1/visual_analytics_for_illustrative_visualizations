#pragma once

#include "Feature.h"
#include <unordered_map>

class ViewpointEntropyFeature : public Feature<float>
{
public:
	ViewpointEntropyFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

