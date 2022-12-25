#pragma once
#include "Feature.h" 

class PixelPositionFeature : public Feature<std::vector<glm::vec2>>
{
public:
	PixelPositionFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

