#pragma once
#include "Feature.h"

class SilhouetteLengthFeature : public Feature<float>
{
public:
	SilhouetteLengthFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

