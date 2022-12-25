#pragma once

#include "../glad/glad.h"
#include "../Uniform.h"
#include "../Model.h"
#include "../shader_utilities.h"

class FramebufferData
{

//atributes
protected:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int framebuffer;
	unsigned int color_texture;
	unsigned int depth_texture;
	
	GLuint elementbuffer;
	Model* model;
	

public:
	std::vector<unsigned int> pixel_color_data;
	std::vector<unsigned int> edge_data;
	std::vector<float> pixel_depth_data;
	std::vector<glm::vec3> unique_vertices;
	glm::vec4 bounding_box;
	float projected_area = 0;
	int visible_triangles = 0;

//methods
private:

public:
	FramebufferData(Model* model);
	virtual ~FramebufferData();
	virtual void refresh(glm::vec3 cameraPosition);

};

