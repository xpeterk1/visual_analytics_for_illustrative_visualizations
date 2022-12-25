#pragma once

#include "Feature.h"

class SilhouetteCurvatureFeature : public Feature<float>
{
public:
	SilhouetteCurvatureFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};