#pragma once
#include "FramebufferData.h"

class NormalFramebufferData : public FramebufferData
{
public:
	GLuint program;
	GLuint elementbuffer;
	Uniform<glm::mat4> view_uniform;
	std::vector<float> normal_pixel_data;

public:
	NormalFramebufferData(Model* model);
	virtual ~NormalFramebufferData();
	virtual void refresh(glm::vec3 cameraPosition);
};

