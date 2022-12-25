#pragma once

#include "glad/glad.h"
#include <glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

using Index = int;
using Lookup = std::map<std::pair<Index, Index>, Index>;

struct Triangle
{
	Index vertex[3];
};


class Icosphere
{
public:
	Icosphere(int subdivisions);
	void draw();

private:
	const float X = 0.525731112119133606f;
	const float Z = 0.850650808352039932f;
	const float N = 0.f;

public:

	std::vector<glm::vec3> vertices =
	{
	  {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
	  {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
	  {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
	};

	std::vector<Triangle> triangles =
	{
	  {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
	  {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
	  {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
	  {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
	};

private:
	Index vertex_for_edge(Lookup& lookup, std::vector<glm::vec3>& vertices, Index first, Index second);
	std::vector<Triangle> subdivide(std::vector<glm::vec3>& vertices, std::vector<Triangle> triangles);
};


