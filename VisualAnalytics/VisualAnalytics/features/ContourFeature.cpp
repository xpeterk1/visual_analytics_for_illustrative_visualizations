#include "ContourFeature.h"

ContourFeature::ContourFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData)
{
	name = "Contour";
	buffer = std::vector<float>(800 * 600);
}

void ContourFeature::compute(glm::vec3 cameraPosition)
{
	for (int i = 0; i < 800 * 600; i++) 
	{
		if (framebufferData->pixel_color_data[i] != 0)
			buffer[i] = 1.0f;
		else
			buffer[i] = 0.0f;
	}
}

std::string ContourFeature::ToString(int idx)
{
	return std::to_string(buffer[idx]);
}
