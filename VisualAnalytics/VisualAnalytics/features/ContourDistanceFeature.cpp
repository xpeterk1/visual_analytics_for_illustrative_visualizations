#include "ContourDistanceFeature.h"
#include "../framebuffers/SilhouetteFramebufferData.h"

ContourDistanceFeature::ContourDistanceFeature(Model* m, FramebufferData* framebufferData) : Feature(m, framebufferData)
{
	name = "Distance from contour";
	buffer = std::vector<float>(800 * 600);
}

void ContourDistanceFeature::compute(glm::vec3 cameraPosition)
{
	SilhouetteFramebufferData* fb = (SilhouetteFramebufferData*)framebufferData;
	buffer.clear();
	buffer.resize(800 * 600);

	auto b = fb->bounding_box;
	
	//top-left down
	for (int x = b.x; x <= b.z; x++)
	{
		for (int y = b.w; y >= b.y; y--)
		{
			int idx = y * 800 + x;

			// current pixel is edge
			if (fb->edge_data[idx] != 0)
			{
				buffer[idx] = 1;
			} else 
			{
				// check top, left, top-left and take min distance
				float top = buffer[(y + 1) * 800 + x];
				float topleft = buffer[(y + 1) * 800 + x - 1];
				float left = buffer[y * 800 + x - 1];

				buffer[idx] = std::min(top, std::min(topleft, left)) + 1;
			}
		}
	}

	//bottom-right up
	for (int x = b.z; x >= b.x; x--)
	{
		for (int y = b.y; y <= b.w; y++)
		{
			int idx = y * 800 + x;

			if (fb->edge_data[idx] != 0.0f)
			{
				buffer[idx] = 1;
			}
			else
			{
				// check bottom, right, bottom-right and current and take min distance
				float bottom = buffer[(y - 1) * 800 + x] + 1;
				float bottomright = buffer[(y - 1) * 800 + x + 1] + 1;
				float right = buffer[y * 800 + x + 1] + 1;
				float current = buffer[idx] == 0 ? 9999999999 : buffer[idx];

				buffer[idx] = std::min(bottom, std::min(bottomright, std::min(right, current)));
			}
		}
	}
}

std::string ContourDistanceFeature::ToString(int idx)
{
	return std::to_string(buffer[idx]);
}
