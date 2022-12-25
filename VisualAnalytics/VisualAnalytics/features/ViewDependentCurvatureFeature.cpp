#include "ViewDependentCurvatureFeature.h"
#include "../framebuffers/ViewDependentCurvatureFramebufferData.h"

ViewDependentCurvatureFeature::ViewDependentCurvatureFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData)
{
	buffer = std::vector<glm::vec3>(800 * 600);
	name = "Min Curvature, Max Curvature, View Dependent Curvature";
}

void ViewDependentCurvatureFeature::compute(glm::vec3 cameraPosition)
{
	ViewDependentCurvatureFramebufferData* fb = (ViewDependentCurvatureFramebufferData*)framebufferData;
	for (int i = 0; i < 800 * 600; i++)
	{
		buffer[i] = fb->view_curvatures_pixels[i];
	}
}

std::string ViewDependentCurvatureFeature::ToString(int idx)
{
	glm::vec3 v = buffer[idx];
	return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z);
}