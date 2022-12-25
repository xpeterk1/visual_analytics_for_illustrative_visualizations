#pragma once
#include "glad/glad.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <glfw3.h>


std::string load_file(std::filesystem::path file_path);
GLuint create_program(std::filesystem::path vertex_path, std::filesystem::path fragment_path);
GLuint create_shader(std::filesystem::path file_path, GLenum shader_type);

