#include "WorldPositionFramebufferData.h"

WorldPositionFramebufferData::WorldPositionFramebufferData(Model* model) : FramebufferData(model),
program(create_program("shaders/triangle_vertex_id.vert", "shaders/world_position.frag")),
view_uniform(program, "view", model->view_matrix) 
{
    // Link model matrices to the program
    Uniform<glm::mat4> projection_uniform(this->program, "projection", model->projection_matrix);
    Uniform<glm::mat4> model_uniform(this->program, "model", model->model_matrix);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, model->vertex_attributes.size() * sizeof(glm::vec3), model->vertex_attributes.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(model->n_vertices * 3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->n_indices * sizeof(unsigned int), &model->indices[0], GL_STATIC_DRAW);

    //recreate and rebound colour texture - it needs to be RGB format (not R32UI)
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    //COLOR TEXTURE
    glDeleteTextures(1, &color_texture);
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);

    //resize color pixel data to fit RGB texture
    world_position_pixel_data.resize(3 * 600 * 800);
}

void WorldPositionFramebufferData::refresh(glm::vec3 cameraPosition)
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

    glDrawElements(GL_TRIANGLES, model->n_triangles * 3, GL_UNSIGNED_INT, 0);

    // COPY DATA FROM TEXTURES
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, world_position_pixel_data.data());

    //glBindTexture(GL_TEXTURE_2D, depth_texture);
    //glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixel_depth_data.data());

    //Compute bounding box
    glm::vec4 box(800, 600, 0, 0); //x1, y1, x2, y2
    for (int x = 0; x < 800; x++)
    {
        for (int y = 0; y < 600; y++)
        {
            float pixel = world_position_pixel_data[3 * (y * 800 + x)];
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

WorldPositionFramebufferData::~WorldPositionFramebufferData()
{
    FramebufferData::~FramebufferData();
    glDeleteProgram(program);
}