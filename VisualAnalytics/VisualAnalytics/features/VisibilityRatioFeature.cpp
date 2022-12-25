#include "VisibilityRatioFeature.h"

VisibilityRatioFeature::VisibilityRatioFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData) 
{
    name = "Visibility ratio";
}

void VisibilityRatioFeature::compute(glm::vec3 cameraPosition)
{
    float sum = 0.0f;

    // iterate through each visible triangle
    std::unordered_set<unsigned int> set(framebufferData->pixel_color_data.begin(), framebufferData->pixel_color_data.end());
    for (const auto& triangle_id : set)
    {
        // background
        if (triangle_id == 0)
            continue;

        // add the area of the visible triangle to the sum
        glm::vec3 p0 = framebufferData->unique_vertices[(triangle_id - 1) * 3];
        glm::vec3 p1 = framebufferData->unique_vertices[(triangle_id - 1) * 3 + 1];
        glm::vec3 p2 = framebufferData->unique_vertices[(triangle_id - 1) * 3 + 2];

        sum += glm::length(glm::cross(p1 - p0, p2 - p0));
    }

    buffer = sum / model->total_area;
}

std::string VisibilityRatioFeature::ToString(int idx)
{
    return std::to_string(buffer);
}