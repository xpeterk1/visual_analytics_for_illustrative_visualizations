#include "ViewDependentCurvatureFramebufferData.h"

ViewDependentCurvatureFramebufferData::ViewDependentCurvatureFramebufferData(Model* model) : FramebufferData(model),
program(create_program("shaders/view_curvature.vert", "shaders/view_curvature.frag")),
view_uniform(program, "view", model->view_matrix)
{
    // Link model matrices to the program
    Uniform<glm::mat4> projection_uniform(this->program, "projection", model->projection_matrix);
    Uniform<glm::mat4> model_uniform(this->program, "model", model->model_matrix);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //bind vertices and their curvatures - first vertices component-wise, then curvatures    
    //vertices
    for (int i = 0; i < model->n_vertices; i++) {
        glm::vec3 vertex = model->vertex_attributes[i];
        vertices.push_back(vertex.x);
        vertices.push_back(vertex.y);
        vertices.push_back(vertex.z);
    }

    // min max principal curvatures
    for (const auto& min_curv : model->min_curvature_values)
        vertices.push_back(min_curv);

    for (const auto& max_curv : model->max_curvature_values)
        vertices.push_back(max_curv);

    //create element buffer
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->n_indices * sizeof(unsigned int), &model->indices[0], GL_STATIC_DRAW);

    //recreate and rebound colour texture - it needs to be RGB format (not R32UI)
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    //COLOR TEXTURE - float RGB texture to hold curvatures
    glDeleteTextures(1, &color_texture);
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);

    //resize color pixel data to fit float texture
    view_curvatures_pixels.resize(600 * 800);
}

void ViewDependentCurvatureFramebufferData::refresh(glm::vec3 cameraPosition)
{
    glUseProgram(program);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view_uniform.LoadData(glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    glActiveTexture(GL_TEXTURE0);
    glBindTextureUnit(0, color_texture);

    //add newly computed view dependent curvatures and load data
    std::vector<float> current_data;
    for (const auto& elem : vertices) {
        current_data.push_back(elem);
    }

    // view dependent curvatures
    for (const auto& curv : model->view_curvatures)
        current_data.push_back(curv);

    glBufferData(GL_ARRAY_BUFFER, current_data.size() * sizeof(float), current_data.data(), GL_STATIC_DRAW);
    
    // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    int offset = (model->n_vertices * 3 * sizeof(float));
    // min curvatures
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)offset);
    glEnableVertexAttribArray(1);

    // max curvatures
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(offset + (model->n_vertices * sizeof(float))));
    glEnableVertexAttribArray(2);

    // view dependent curvature
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(offset + 2 * (model->n_vertices * sizeof(float))));
    glEnableVertexAttribArray(3);
   
    //draw
    glDrawElements(GL_TRIANGLES, model->n_triangles * 3, GL_UNSIGNED_INT, 0);

    // COPY DATA FROM TEXTURES
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, view_curvatures_pixels.data());

    // RETURN OLD FRAMEBUFFER
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ViewDependentCurvatureFramebufferData::~ViewDependentCurvatureFramebufferData()
{
    FramebufferData::~FramebufferData();
    glDeleteProgram(program);
}