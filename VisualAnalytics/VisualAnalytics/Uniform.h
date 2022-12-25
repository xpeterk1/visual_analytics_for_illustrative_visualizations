#pragma once

#include <string>
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"
#include "utils.h"

template <typename T>
class Uniform
{

private:
	std::string name;
	GLuint program;
	int location_id;

public:

	Uniform<T>(GLuint program, std::string uniform_name) 
	{
		this->program = program;
		this->name = uniform_name;
		this->location_id = glGetUniformLocation(program, uniform_name.c_str());
		get_gl_err(__FILE__, __LINE__);
	}

	Uniform<T>(GLuint program, std::string uniform_name, const T& data) : Uniform(program, uniform_name)
	{
		LoadData(data);
		get_gl_err(__FILE__, __LINE__);
	}

	void LoadData(const T& vec3) requires std::is_same<T, glm::vec3>::value
	{
		glUseProgram(program);
		glUniform3fv(location_id, 1, glm::value_ptr(vec3));
		get_gl_err(__FILE__, __LINE__);
	}

	void LoadData(const T& vec4) requires std::is_same<T, glm::vec4>::value
	{
		glUseProgram(program);
		glUniform4fv(location_id, 1, glm::value_ptr(vec4));
		get_gl_err(__FILE__, __LINE__);
	}

	void LoadData(const T& mat4) requires std::is_same<T, glm::mat4>::value
	{
		glUseProgram(program);
		glUniformMatrix4fv(location_id, 1, GL_FALSE, glm::value_ptr(mat4));
		get_gl_err(__FILE__, __LINE__);
	}

	void LoadData(const T& fl) requires std::is_same<T, float>::value
	{
		glUseProgram(program);
		glUniform1f(location_id, fl);
		get_gl_err(__FILE__, __LINE__);
	}

	void LoadData(const T& i) requires std::is_same<T, int>::value
	{
		glUseProgram(program);
		glUniform1i(location_id, i);
		get_gl_err(__FILE__, __LINE__);
	}

};

