#include "ViewpointEntropyFeature.h"

ViewpointEntropyFeature::ViewpointEntropyFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData) 
{
	name = "Viewpoint entropy";
}

void ViewpointEntropyFeature::compute(glm::vec3 cameraPosition)
{
	// triangle ID -> accumulated area
	std::unordered_map<int, float> area_map;

	for (int x = framebufferData->bounding_box.x; x <= framebufferData->bounding_box.z; x++)
	{
		for (int y = framebufferData->bounding_box.y; y <= framebufferData->bounding_box.w; y++)
		{
			int triangleID = framebufferData->pixel_color_data[y * 800 + x];

			//background
			if (triangleID == 0)
				continue;

			if (!area_map.contains(triangleID))
				area_map[triangleID] = framebufferData->pixel_depth_data[y * 800 + x];
			else
				area_map[triangleID] += framebufferData->pixel_depth_data[y * 800 + x];
		}
	}

	float sum = 0.0f;
	for (const auto& triangle_id_area : area_map)
	{
		sum += (triangle_id_area.second / framebufferData->projected_area) * log(triangle_id_area.second / framebufferData->projected_area);
	}

	buffer = -1 * sum;
}

std::string ViewpointEntropyFeature::ToString(int idx)
{
	return std::to_string(buffer);
}