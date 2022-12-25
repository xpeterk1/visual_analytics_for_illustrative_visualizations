#include "SilhouetteCurvatureFeature.h"
#include "../framebuffers/SilhouetteFramebufferData.h"

SilhouetteCurvatureFeature::SilhouetteCurvatureFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData) 
{
    name = "Silhouette Curvature";
}

void SilhouetteCurvatureFeature::compute(glm::vec3 cameraPosition)
{
    SilhouetteFramebufferData* fb = (SilhouetteFramebufferData *)framebufferData;

    int n_curvatures = 0;
    float curvature = 0;
    for (int i = 0; i < 2 * 600 * 800; i += 2) 
    {
        curvature += fb->curvature_data[i] / 2.0f;
        n_curvatures += fb->curvature_data[i + 1];
    }

    curvature /= n_curvatures;

    buffer = curvature;
}

std::string SilhouetteCurvatureFeature::ToString(int idx)
{
    return std::to_string(buffer);
}