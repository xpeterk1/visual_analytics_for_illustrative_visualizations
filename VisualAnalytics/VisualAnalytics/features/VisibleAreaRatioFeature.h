#pragma once
#include "Feature.h"

class VisibleAreaRatioFeature : public Feature<float>
{

public:
	VisibleAreaRatioFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

