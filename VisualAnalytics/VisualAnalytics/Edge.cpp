#include "Edge.h"

#include <stdexcept>

Edge::Edge(glm::vec3 a, glm::vec3 b)
{
	glm::vec3 p;
	glm::vec3 q;

	if (a.x < b.x) {
		p = a;
		q = b;
	}
	else if (a.x > b.x) {
		p = b;
		q = a;
	}
	else if (a.y < b.y) {
		p = a;
		q = b;
	}
	else if (a.y > b.y) {
		p = b;
		q = a;
	}
	else if (a.z < b.z) {
		p = a;
		q = b;
	}
	else if (a.z > b.z) {
		p = b;
		q = a;
	}
	else {
		// both points are the same
		p = a;
		q = b;
	}

	this->p = p;
	this->q = q;

	this->length = glm::length(p - q);
}

void Edge::add_area(float area)
{
	if (this->triangle_areas.size() >= 2) {
		throw std::logic_error(">=2 areas");
	}

	this->triangle_areas.push_back(area);
}

float Edge::get_areas() 
{
	float result = 0.0f;
	for (float a : this->triangle_areas) result += a;
	return result;
}

bool Edge::endpoints_eq(const Edge& rhs) const
{
	return this->p == rhs.p && this->q == rhs.q;
}

glm::vec3 Edge::get_second_point(glm::vec3 first_point) 
{
	if (first_point.x == p.x && first_point.y == p.y && first_point.z == p.z)
		return q;
	return p;
}