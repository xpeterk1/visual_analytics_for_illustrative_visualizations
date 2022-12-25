#include "IdDepthFramebufferData.h"

IdDepthFramebufferData::IdDepthFramebufferData(Model* model) : FramebufferData(model), 
program(create_program("shaders/triangle_vertex_id.vert", "shaders/triangle_vertex_id.frag")),
view_uniform(program, "view", model->view_matrix)
{
    // Link model matrices to the program
    Uniform<glm::mat4> projection_uniform(this->program, "projection", model->projection_matrix);
    Uniform<glm::mat4> model_uniform(this->program, "model", model->model_matrix);
}

void IdDepthFramebufferData::refresh(glm::vec3 cameraPosition)
{
    glUseProgram(program);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view_uniform.LoadData(glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    glActiveTexture(GL_TEXTURE0);
    glBindTextureUnit(0, color_texture);

    glDrawElements(GL_TRIANGLES, model->n_triangles * 3, GL_UNSIGNED_INT, 0);

    // COPY DATA FROM TEXTURES
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, pixel_color_data.data());

    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixel_depth_data.data());

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

    bounding_box = box;

    // RETURN OLD FRAMEBUFFER
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

IdDepthFramebufferData::~IdDepthFramebufferData()
{
    FramebufferData::~FramebufferData();
    glDeleteProgram(program);
}