#include "utils.h"


void get_gl_err(const char* file, int line_number) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << file << " " << line_number << ": " << err << std::endl;
	}
}

/// <summary>
/// Standardize components of float array
/// </summary>
/// <param name="values"></param>
void standardize(float* values)
{
	// compute mean
	float sum = 0.0;
	int n = 0;

	for (int i = 0; i < 800 * 600; i++)
	{
		if (values[i] != 0.0f)
		{
			sum += values[i];
			n++;
		}
	}

	float mean = sum / n;

	// compute std
	float tmp = 0.0;
	for (int i = 0; i < 800 * 600; i++) {
		tmp += pow(values[i] - mean, 2);
	}

	float std = sqrt(tmp / n);

	// transform data
	for (int i = 0; i < 800 * 600; i++) {
		if (values[i] != 0.0)
		{
			values[i] = (values[i] - mean) / std;
		}
	}
}

/// <summary>
/// Standardize components of vector array - channel-wise
/// </summary>
/// <param name="values"></param>
void standardize(glm::vec3* values)
{
	// compute means
	glm::vec3 sum(0.0);
	int n = 0;

	for (int i = 0; i < 800 * 600; i++)
	{
		if (values[i].x != 0.0f)
			sum.x += values[i].x;

		if (values[i].y != 0.0f)
			sum.y += values[i].y;

		if (values[i].z != 0.0f)
			sum.z += values[i].z;

		if (values[i].x != 0.0f || values[i].y != 0.0f || values[i].z != 0.0f)
			n++;
	}

	glm::vec3 mean(sum.x / n, sum.y / n, sum.z / n);

	// compute std
	glm::vec3 tmp(0.0);
	for (int i = 0; i < 800 * 600; i++) {

		tmp.x += pow(values[i].x - mean.x, 2);
		tmp.y += pow(values[i].y - mean.y, 2);
		tmp.z += pow(values[i].z - mean.z, 2);
	}

	glm::vec3 std(tmp.x / n, tmp.y / n, tmp.z / n);

	// transform data
	for (int i = 0; i < 800 * 600; i++) {
		values[i].x = (values[i].x - mean.x) / std.x;
		values[i].y = (values[i].y - mean.y) / std.y;
		values[i].z = (values[i].z - mean.z) / std.z;
	}
}