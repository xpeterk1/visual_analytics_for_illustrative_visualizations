#pragma once
#include "FramebufferData.h"
class SilhouetteFramebufferData : public FramebufferData
{
public:
	SilhouetteFramebufferData(Model* model);
	virtual ~SilhouetteFramebufferData();
	virtual void refresh(glm::vec3 cameraPosition);

private:
	GLuint silhouetteProgram;
	GLuint contours_program; 
	GLuint edge_detection_program;
	GLuint curvature_program;
	GLuint discontinuity_program;
	unsigned int linesVAO;
	unsigned int linesVBO;
	unsigned int quadVAO;
	unsigned int edgeFramebuffer;
	unsigned int curvatureFramebuffer;
	unsigned int edgeTexture;
	unsigned int curvatureTexture;

public:
	Uniform<glm::mat4> view_uniform_silhouette;
	Uniform<glm::mat4> view_uniform_contour;
	std::vector<int> curvature_data;
	std::vector<int> discontinuity_data;

};

