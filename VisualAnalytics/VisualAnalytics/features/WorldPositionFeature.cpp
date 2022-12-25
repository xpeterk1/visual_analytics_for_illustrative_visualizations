#include "WorldPositionFeature.h"
#include "../framebuffers/WorldPositionFramebufferData.h"

WorldPositionFeature::WorldPositionFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData)
{
	buffer = std::vector<glm::vec3>(800 * 600);
	name = "World Position X, World Position Y, World Position Z";
}

void WorldPositionFeature::compute(glm::vec3 cameraPosition)
{
	WorldPositionFramebufferData* fb = (WorldPositionFramebufferData*)framebufferData;
	for (int i = 0; i < 800 * 600; i++)
	{
		float x = fb->world_position_pixel_data[3 * i];
		float y = fb->world_position_pixel_data[3 * i + 1];
		float z = fb->world_position_pixel_data[3 * i + 2];

		buffer[i] = glm::vec3(x, y, z);
	}
}

std::string WorldPositionFeature::ToString(int idx)
{
	glm::vec3 v = buffer[idx];
	return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z);
}