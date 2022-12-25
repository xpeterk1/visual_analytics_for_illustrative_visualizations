#include "FramebufferData.h"

#include <algorithm>
#include <iostream>

FramebufferData::FramebufferData(Model* model)
{
    if (model == nullptr)
        return;

    this->model = model;
    pixel_color_data.resize(800 * 600);
    pixel_depth_data.resize(800 * 600);

    // Create unique instrance of vertex for each triangle
    for (const auto& triangle : model->triangle_indices)
    {
        unique_vertices.push_back(glm::vec3(model->vertices[triangle.x]));
        unique_vertices.push_back(glm::vec3(model->vertices[triangle.y]));
        unique_vertices.push_back(glm::vec3(model->vertices[triangle.z]));
    }

    // Generate VAO + VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Load data
    glBufferData(GL_ARRAY_BUFFER, unique_vertices.size() * sizeof(glm::vec3), unique_vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    //COLOR TEXTURE
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, 800, 600, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);

    //DEPTH TEXTURE
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);

    // return the original framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // IDs for model vertices
    std::vector<unsigned int> ids(unique_vertices.size());
    for (int i = 0; i < unique_vertices.size(); i++)
        ids[i] = i;

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, unique_vertices.size() * sizeof(unsigned int), &ids[0], GL_STATIC_DRAW);
}

FramebufferData::~FramebufferData() 
{
    glDeleteBuffers(1, &VBO); 
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &color_texture);
    glDeleteTextures(1, &depth_texture); 
}

void FramebufferData::refresh(glm::vec3 cameraPosition)
{

}
