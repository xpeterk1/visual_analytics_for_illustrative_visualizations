#pragma once
#include "Feature.h" 
class WorldPositionFeature : public Feature<std::vector<glm::vec3>>
{
public:
	WorldPositionFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

