#include "DepthMaximizingFeature.h"

DepthMaximizingFeature::DepthMaximizingFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData) 
{
	name = "Depth Maximizing";
}

void DepthMaximizingFeature::compute(glm::vec3 cameraPosition)
{
	float normalized_area = 0.0f;
	float max_depth = 0.0f;
	for (int x = framebufferData->bounding_box.x; x <= framebufferData->bounding_box.z; x++)
	{
		for (int y = framebufferData->bounding_box.y; y <= framebufferData->bounding_box.w; y++)
		{
			unsigned int triangle_id = framebufferData->pixel_color_data[y * 800 + x];
			float depth = framebufferData->pixel_depth_data[y * 800 + x];

			if (triangle_id != 0) 
			{
				normalized_area++;

				if (depth > max_depth)
					max_depth = depth;
			}
		}
	}

	normalized_area /= (800 * 600);

	buffer = (1.0 / 3.0) * normalized_area + (1.0 / 3.0) * max_depth + (1.0 / 3.0) * (1 - abs(max_depth - normalized_area));
}

std::string DepthMaximizingFeature::ToString(int idx)
{
	return std::to_string(buffer);
}