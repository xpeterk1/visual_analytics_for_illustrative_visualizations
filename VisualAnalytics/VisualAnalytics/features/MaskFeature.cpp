#include "MaskFeature.h"

MaskFeature::MaskFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData)
{
	name = "Mask";
	buffer = std::vector<float>(800 * 600);
}

void MaskFeature::compute(glm::vec3 cameraPosition)
{
	for (int i = 0; i < 800 * 600; i++) {
		if (framebufferData->pixel_depth_data[i] == 1.0f) buffer[i] = 0;
		else buffer[i] = 1;
	}
}

std::string MaskFeature::ToString(int idx)
{
	return std::to_string(buffer[idx]);
}
