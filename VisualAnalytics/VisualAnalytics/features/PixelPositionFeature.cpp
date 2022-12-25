#include "PixelPositionFeature.h"

PixelPositionFeature::PixelPositionFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData) 
{
	buffer = std::vector<glm::vec2>(800 * 600);
	name = "Pixel Position X, Pixel Position Y";
}

void PixelPositionFeature::compute(glm::vec3 cameraPosition)
{
	for (int i = 0; i < 800 * 600; i++) 
	{
		float x = (i % 800) / 800.0f;
		float y = (i / 800) / 600.0f;
		buffer[i] = glm::vec2(x, y);
	}
}

std::string PixelPositionFeature::ToString(int idx)
{
	glm::vec2 v = buffer[idx];
	return std::to_string(v.x) + ", " + std::to_string(v.y);
}