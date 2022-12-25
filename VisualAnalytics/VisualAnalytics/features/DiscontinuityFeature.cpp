#include "DiscontinuityFeature.h"
#include "../framebuffers/SilhouetteFramebufferData.h"

DiscontinuityFeature::DiscontinuityFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData)
{
	buffer = std::vector<float>(800 * 600);
	name = "Discontinuity";
}

void DiscontinuityFeature::compute(glm::vec3 cameraPosition) 
{
	SilhouetteFramebufferData* fb = (SilhouetteFramebufferData*)framebufferData;
	for (int i = 0; i < 800 * 600; i++)
	{
		buffer[i] = fb->discontinuity_data[2 * i];
	}
}

std::string DiscontinuityFeature::ToString(int idx)
{
	return std::to_string(buffer[idx]);
}