#pragma once

#include <iostream>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <vector>

void get_gl_err(const char* file, int line_number);

void standardize(float* values);
void standardize(glm::vec3* values);