#pragma once
#include "Feature.h" 
class MaskFeature : public Feature<std::vector<float>>
{

public:
	MaskFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);

};

