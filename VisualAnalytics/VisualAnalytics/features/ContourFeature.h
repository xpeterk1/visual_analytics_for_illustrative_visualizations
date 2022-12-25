#pragma once
#include "Feature.h" 
#include<algorithm>

class ContourFeature : public Feature<std::vector<float>>
{
public:
	ContourFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

