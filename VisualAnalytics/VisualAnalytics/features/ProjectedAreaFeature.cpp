#include "ProjectedAreaFeature.h"

ProjectedAreaFeature::ProjectedAreaFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData) 
{
    name = "Projected Area";
}

void ProjectedAreaFeature::compute(glm::vec3 cameraPosition)
{
    // Read all pixels in the depth buffer and sum up the values
    float area = 0;
    for (int x = framebufferData->bounding_box.x; x <= framebufferData->bounding_box.z; x++)
    {
        for (int y = framebufferData->bounding_box.y; y <= framebufferData->bounding_box.w; y++)
        {
            if (framebufferData->pixel_depth_data[y * 800 + x] < 1.0f)
            {
                area += framebufferData->pixel_depth_data[y * 800 + x];
            }
        }
    }

    buffer = area;
    framebufferData->projected_area = area;
}

std::string ProjectedAreaFeature::ToString(int idx)
{
    return std::to_string(buffer);
}