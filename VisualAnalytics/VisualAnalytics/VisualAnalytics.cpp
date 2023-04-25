#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "glad/glad.h"
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include "Camera.h"
#include "Model.h"
#include "shader_utilities.h"
#include "Uniform.h"
#include "utils.h"
#include "features/VisibleTrianglesFeature.h"
#include "Featureset.h"
#include "Icosphere.h"
#include "Exporter.h"

#include "imgui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
void DrawGUI();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera main_camera(400, 300, 0.0f, 0.0f, 10.0f);
Icosphere icosphere(2);
Exporter exporter;

bool is_sampling = false;
int sampling_index = 0;

int main()
{
    Model model("models/f-16.obj");
    glm::vec4 lightPos(0.2f, 1.0f, 0.8f, 2.0f);
    float shininess = 5.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Visual Analytics", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, onKeyPress);

    // load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint contours_program = create_program("shaders/contours.vert", "shaders/contours.frag");
    GLuint silhouette_program = create_program("shaders/silhouette.vert", "shaders/silhouette.frag");
    GLuint mesh_program = create_program("shaders/main.vert", "shaders/main.frag");


    //Init ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    //====================================

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, model.vertex_attributes.size() * sizeof(glm::vec3), model.vertex_attributes.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(model.n_vertices * 3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.n_indices * sizeof(unsigned int), &model.indices[0], GL_STATIC_DRAW);

    //====================================

    unsigned int linesVAO;
    glGenVertexArrays(1, &linesVAO);

    unsigned int linesVBO;
    glGenBuffers(1, &linesVBO);

    glBindVertexArray(linesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, linesVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);


    //====================================
    //FRAMEBUFFER STUFF

    //FRAMEBUFFER
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    //COLOR TEXTURE
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    // return the original framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //====================================

    glm::mat4 model_mat(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.001f, 1000.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(1.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUseProgram(contours_program);

    // create uniforms
    Uniform<glm::mat4> model_uniform(contours_program, "model", model_mat);
    Uniform<glm::mat4> view_uniform(contours_program, "view", view);
    Uniform<glm::mat4> projection_uniorm(contours_program, "projection", projection);

    Uniform<float> screen_width_uniform(contours_program, "screenWidth", static_cast<float>(SCR_WIDTH));
    Uniform<float> screen_height_uniform(contours_program, "screenHeight", static_cast<float>(SCR_HEIGHT));

    glUseProgram(mesh_program);

    Uniform<glm::mat4> mesh_model_uniform(mesh_program, "model", model_mat);
    Uniform<glm::mat4> mesh_view_uniform(mesh_program, "view", view);
    Uniform<glm::mat4> mesh_projection_uniorm(mesh_program, "projection", projection);

    Uniform<glm::mat4> silhouette_model_uniform(silhouette_program, "model", model_mat);
    Uniform<glm::mat4> silhouette_view_uniform(silhouette_program, "view", view);
    Uniform<glm::mat4> silhouette_projection_uniorm(silhouette_program, "projection", projection);

    Uniform<glm::vec3> camera_position_uniform(mesh_program, "cameraPos", main_camera.get_camera_eye());
    Uniform<glm::vec4> light_position_uniform(mesh_program, "lightPos", lightPos);
    Uniform<float> shininess_uniform(mesh_program, "shininess", shininess);

    Uniform<int> framebuffer_color_texture_uniform(mesh_program, "mask", 0);

    std::cout << "main render loop" << std::endl;

    model.model_matrix = model_mat;
    model.view_matrix = view;
    model.projection_matrix = projection;
    FeatureSet fs(&model, main_camera.get_camera_eye());

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        if (is_sampling) {
            main_camera.set_camera(icosphere.vertices[sampling_index]);
        }

        //New ImGUI Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Draw GUI
        DrawGUI();

        view = glm::lookAt(main_camera.get_camera_eye(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        view_uniform.LoadData(view);
        mesh_view_uniform.LoadData(view);
        silhouette_view_uniform.LoadData(view);

        model.compute_view_dependent_curvature(main_camera.get_camera_eye());
        model.compute_contours(main_camera.get_camera_eye());
        model.view_matrix = view;
        
        //CONTOURS RENDER START
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
        glBufferData(GL_ARRAY_BUFFER, model.contours.size() * sizeof(glm::vec3), model.contours.data(), GL_DYNAMIC_DRAW);
        glBindVertexArray(linesVAO);
        glUseProgram(contours_program);
        glLineWidth(2.0f);
        glDrawArrays(GL_LINES, 0, model.contours.size());
        //CONTOURS RENDER END

        //MODEL RENDER START
        //draw mesh to the framebuffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        glUseProgram(silhouette_program);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, model.n_indices, GL_UNSIGNED_INT, 0);
        
        // second pass to compute contour
        glUseProgram(mesh_program);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTextureUnit(0, textureColorbuffer);
        glDrawElements(GL_TRIANGLES, model.n_indices, GL_UNSIGNED_INT, 0);
        //MODEL RENDER END

        if (is_sampling) {
            if (sampling_index == 0)
                exporter.clear_all();

            std::cout << "Exporting viewpoint " << sampling_index + 1 << "/" << icosphere.vertices.size() << std::endl;
            fs.recompute_features(main_camera.get_camera_eye());
            exporter.add_features(&fs);
            sampling_index++;

            // All imported, normalize, perform subset selection and export to csv
            if (sampling_index + 1 == icosphere.vertices.size())
            {
                is_sampling = false;
                exporter.normalize();
                auto reduced_data = exporter.reduce_rows();
                exporter.export_to_csv("export.csv", &fs, reduced_data);
                std::cout << "Export completed " << std::endl;
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteFramebuffers(1, &framebuffer);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double x, double y) 
{
    main_camera.on_mouse_move(x, y);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    main_camera.on_mouse_button_down(button, action);
}

void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //export current viewport
    if (key == GLFW_KEY_SPACE && action == 1) 
    {
        sampling_index = 0;
        is_sampling = true;
    }
}

void DrawGUI()
{
    ImGui::Begin("Main Menu");

    ImGui::Text("Kernel Density Estimation");

    ImGui::End();
}