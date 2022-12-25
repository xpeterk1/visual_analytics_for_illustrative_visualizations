#include "NormalFeature.h"
#include "../framebuffers/NormalFramebufferData.h"


NormalFeature::NormalFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData)
{
	buffer = std::vector<glm::vec3>(800 * 600);
	name = "Normal X, Normal Y, Normal Z";
}

void NormalFeature::compute(glm::vec3 cameraPosition)
{
	NormalFramebufferData* fb = (NormalFramebufferData*)framebufferData;
	for (int i = 0; i < 800 * 600; i++)
	{
		float x = fb->normal_pixel_data[3*i];
		float y = fb->normal_pixel_data[3*i + 1];
		float z = fb->normal_pixel_data[3*i + 2];

		buffer[i] = glm::vec3(x, y, z);
	}
}

std::string NormalFeature::ToString(int idx)
{
	glm::vec3 v = buffer[idx];
	return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z);
}