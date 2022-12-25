#include "VisibleTrianglesFeature.h"

VisibleTrianglesFeature::VisibleTrianglesFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData) 
{
    name = "Visible triangles";
}

void VisibleTrianglesFeature::compute(glm::vec3 cameraPosition)
{
    // Read all pixels in the color buffer and count unique IDs
    std::unordered_set<unsigned int> triangle_set;

    int counter = 0;
    for (int x = framebufferData->bounding_box.x; x <= framebufferData->bounding_box.z; x++)
    {
        for (int y = framebufferData->bounding_box.y; y <= framebufferData->bounding_box.w; y++)
        {
            unsigned int triangle_id = framebufferData->pixel_color_data[y * 800 + x];

            if (triangle_id != 0) {
                triangle_set.insert(triangle_id);
            }
        }
    }

    buffer = (float)triangle_set.size();
    framebufferData->visible_triangles = triangle_set.size();
}

std::string VisibleTrianglesFeature::ToString(int idx)
{
    return std::to_string(buffer);
}