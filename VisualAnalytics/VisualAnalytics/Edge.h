#pragma once

#include <glm/glm.hpp>
#include <vector>

class Edge
{
public:
	
	/// <summary>
	/// Edge points
	/// </summary>
	glm::vec3 p;
	glm::vec3 q;

	/// <summary>
	/// Length of the edge
	/// </summary>
	float length;

	/// <summary>
	/// Areas of triangles, that contain the edge
	/// </summary>
	std::vector<float> triangle_areas;

	Edge(glm::vec3 P, glm::vec3 Q);
	void add_area(float area);
	float get_areas();
	glm::vec3 get_second_point(glm::vec3 first_point);
	bool endpoints_eq(const Edge& rhs) const;
};


