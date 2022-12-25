#pragma once
#include "FramebufferData.h"
class WorldPositionFramebufferData : public FramebufferData
{
public:
	GLuint program;
	GLuint elementbuffer;
	Uniform<glm::mat4> view_uniform;
	std::vector<float> world_position_pixel_data;

public:
	WorldPositionFramebufferData(Model* model);
	virtual ~WorldPositionFramebufferData();
	virtual void refresh(glm::vec3 cameraPosition);
};

