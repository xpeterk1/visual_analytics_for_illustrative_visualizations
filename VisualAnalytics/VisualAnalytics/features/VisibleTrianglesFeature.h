#pragma once

#include "Feature.h"
#include "../shader_utilities.h"
#include "../Uniform.h"

#include <vector>
#include "../glad/glad.h"

#include <unordered_set>
#include "../utils.h"

class VisibleTrianglesFeature : public Feature<float>
{

public:
	VisibleTrianglesFeature(Model* m, FramebufferData* framebufferData);
	virtual void compute(glm::vec3 cameraPosition);
	virtual std::string ToString(int idx);
};

