#pragma once
#include "FramebufferData.h"

class ViewDependentCurvatureFramebufferData : public FramebufferData
{
public:
	GLuint program;
	GLuint elementbuffer;
	Uniform<glm::mat4> view_uniform;
	std::vector<glm::vec3> view_curvatures_pixels;

private:
	std::vector<float> vertices;

public:
	ViewDependentCurvatureFramebufferData(Model* model);
	virtual ~ViewDependentCurvatureFramebufferData();
	virtual void refresh(glm::vec3 cameraPosition);
};

