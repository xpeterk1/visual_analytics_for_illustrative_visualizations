#include "SilhouetteLengthFeature.h"

SilhouetteLengthFeature::SilhouetteLengthFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData) 
{
    name = "Silhouette Length";
}

void SilhouetteLengthFeature::compute(glm::vec3 cameraPosition)
{
    // Count the number of pixels of suggestive contour or object border silhouette
    float counter = 0.0;
    for (int x = framebufferData->bounding_box.x; x <= framebufferData->bounding_box.z; x++) 
    {
        for (int y = framebufferData->bounding_box.y; y <= framebufferData->bounding_box.w; y++) 
        {
            if (framebufferData->pixel_color_data[y * 800 + x] != 0)
                counter++;
        }
    }

    buffer = counter;
}

std::string SilhouetteLengthFeature::ToString(int idx)
{
    return std::to_string(buffer);
}