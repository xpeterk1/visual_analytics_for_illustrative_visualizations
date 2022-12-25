#pragma once
#include "FramebufferData.h"

class IdDepthFramebufferData : public FramebufferData
{
public:
	GLuint program;
	Uniform<glm::mat4> view_uniform;

public:
	IdDepthFramebufferData(Model* model);
	virtual ~IdDepthFramebufferData();
	virtual void refresh(glm::vec3 cameraPosition);
};

