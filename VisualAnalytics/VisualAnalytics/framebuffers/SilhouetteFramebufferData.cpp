#include "SilhouetteFramebufferData.h"

SilhouetteFramebufferData::SilhouetteFramebufferData(Model* model) : FramebufferData(model), 
silhouetteProgram(create_program("shaders/silhouette.vert", "shaders/silhouette1i.frag")),
contours_program(create_program("shaders/contours.vert", "shaders/contours1i.frag")),
edge_detection_program(create_program("shaders/edge_detection.vert", "shaders/edge_detection.frag")),
curvature_program(create_program("shaders/edge_curvature.vert", "shaders/edge_curvature.frag")),
discontinuity_program(create_program("shaders/discontinuity.vert", "shaders/discontinuity.frag")),
view_uniform_silhouette(silhouetteProgram, "view", model->view_matrix),
view_uniform_contour(contours_program, "view", model->view_matrix)
{
	Uniform<glm::mat4> projection_uniform_silhouette(this->silhouetteProgram, "projection", model->projection_matrix);
	Uniform<glm::mat4> model_uniform_silhouette(this->silhouetteProgram, "model", model->model_matrix);

	Uniform<glm::mat4> projection_uniform_contour(this->contours_program, "projection", model->projection_matrix);
	Uniform<glm::mat4> model_uniform_contour(this->contours_program, "model", model->model_matrix);

	glGenVertexArrays(1, &linesVAO);
	glGenBuffers(1, &linesVBO);

	glBindVertexArray(linesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glGenFramebuffers(1, &edgeFramebuffer);
	glGenFramebuffers(1, &curvatureFramebuffer);

	//-------------------- QUAD --------------------//
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	//----------------------------------------------//


	//EDGE TEXTURE
	glBindFramebuffer(GL_FRAMEBUFFER, edgeFramebuffer);
	glGenTextures(1, &edgeTexture);
	glBindTexture(GL_TEXTURE_2D, edgeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, 800, 600, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, edgeTexture, 0);

	//Set clamp and filter mode for color texture
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, 0);


	//CURVATURE TEXTURE
	glGenTextures(1, &curvatureTexture);
	glBindTexture(GL_TEXTURE_2D, curvatureTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32I, 800, 600, 0, GL_RG_INTEGER, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, curvatureFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, curvatureTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	curvature_data.resize(2 * 800 * 600); 
	discontinuity_data.resize(2 * 800 * 600);
	edge_data.resize(800 * 600);
}

SilhouetteFramebufferData::~SilhouetteFramebufferData()
{
	FramebufferData::~FramebufferData();
	glDeleteProgram(silhouetteProgram);
	glDeleteProgram(contours_program);
	glDeleteProgram(curvature_program);
	glDeleteProgram(edge_detection_program);
	glDeleteProgram(discontinuity_program);
	glDeleteFramebuffers(1, &edgeFramebuffer);
	glDeleteFramebuffers(1, &curvatureFramebuffer);
	glDeleteTextures(1, &edgeTexture);
	glDeleteTextures(1, &curvatureTexture);
}

void SilhouetteFramebufferData::refresh(glm::vec3 cameraPosition)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	view_uniform_silhouette.LoadData(glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	view_uniform_contour.LoadData(glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTextureUnit(0, color_texture);

	//Draw Model
	glUseProgram(silhouetteProgram);
	glDrawElements(GL_TRIANGLES, model->n_triangles * 3, GL_UNSIGNED_INT, 0);

	//Draw suggetive contours
	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
	glBufferData(GL_ARRAY_BUFFER, model->contours.size() * sizeof(glm::vec3), model->contours.data(), GL_DYNAMIC_DRAW);
	glBindVertexArray(linesVAO);
	glUseProgram(contours_program);
	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, model->contours.size());

	//EDGE DETECTION
	glBindFramebuffer(GL_FRAMEBUFFER, edgeFramebuffer);
	glUseProgram(edge_detection_program);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTextureUnit(0, color_texture);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// copy edges
	glBindTexture(GL_TEXTURE_2D, edgeTexture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, edge_data.data());

	//CURVATURE DETECTION
	glBindFramebuffer(GL_FRAMEBUFFER, curvatureFramebuffer);
	glBindTextureUnit(0, edgeTexture);
	glUseProgram(curvature_program);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	// COPY DATA FROM TEXTURES
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, pixel_color_data.data());

	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixel_depth_data.data());

	glBindTexture(GL_TEXTURE_2D, curvatureTexture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RG_INTEGER, GL_INT, curvature_data.data());

	glBindTextureUnit(0, edgeTexture);
	glUseProgram(discontinuity_program);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//Compute bounding box
	glm::vec4 box(800, 600, 0, 0); //x1, y1, x2, y2
	for (int x = 0; x < 800; x++) 
	{
		for (int y = 0; y < 600; y++)
		{
			unsigned int pixel = pixel_color_data[y * 800 + x];
			if (pixel != 0)
			{
				if (x < box.x) box.x = x;
				if (x > box.z) box.z = x;
				if (y < box.y) box.y = y;
				if (y > box.w) box.w = y;
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, curvatureTexture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RG_INTEGER, GL_INT, discontinuity_data.data());

	bounding_box = box;

	// RETURN OLD FRAMEBUFFER
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
