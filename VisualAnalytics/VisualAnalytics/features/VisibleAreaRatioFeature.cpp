#include "VisibleAreaRatioFeature.h"

VisibleAreaRatioFeature::VisibleAreaRatioFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData) 
{
    name = "Visible Area Ratio";
}

void VisibleAreaRatioFeature::compute(glm::vec3 cameraPosition)
{
    // Combine the number of visible triangles with the total visible area

    buffer = (float)framebufferData->visible_triangles / model->n_triangles + framebufferData->projected_area / 800.0f * 600.0f;
}

std::string VisibleAreaRatioFeature::ToString(int idx)
{
    return std::to_string(buffer);
}