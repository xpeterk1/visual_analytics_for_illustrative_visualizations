#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

#include "tinyobjloader/tiny_obj_loader.h"
#include "Eigen/Dense"

#define _PI 3.14159265

class Model
{

	// Attributes
private:

	/// <summary>
	/// Model file path
	/// </summary>
	std::string file_path;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	/// <summary>
	/// Normals of each triangle
	/// </summary>
	std::vector<glm::vec3> triangle_normals;

	std::vector<glm::vec3> normals;

	//CONSTANTS
	double mark_length = 0.02;
	double suggestive_diff_thresh_min = 20.0;
	double suggestive_diff_thresh_max = 600.0;
	double suggestive_diff_thresh_incr = 20.0;
	double suggestive_angle_thresh_min = 6.0;
	double suggestive_angle_thresh_max = 20.0;
	double suggestive_angle_thresh_incr = 2.0;
	unsigned int contour_length_thresh_min = 1;
	unsigned int contour_length_thresh_max = 5;
	unsigned int contour_length_thresh_incr = 1;

	double suggestive_diff_thresh = suggestive_diff_thresh_min;
	double suggestive_angle_thresh = suggestive_angle_thresh_min;
	unsigned int contour_length_thresh = contour_length_thresh_min;

public:
	/// <summary>
	/// All indices into vertices vector
	/// </summary>
	std::vector<unsigned int> indices;

	/// <summary>
	/// Vertex attributes behind each other
	/// </summary>
	std::vector<glm::vec3> vertex_attributes;

	/// <summary>
	/// Total count of vertices
	/// </summary>
	std::size_t n_vertices;

	/// <summary>
	/// Total count of indices
	/// </summary>
	std::size_t n_indices;

	/// <summary>
	/// Total number of faces present in the mesh
	/// </summary>
	std::size_t n_triangles;

	/// <summary>
	/// Vertices of one triangle are groupped into one uvec3
	/// </summary>
	std::vector<glm::uvec3> triangle_indices;

	/// <summary>
	/// All vertices
	/// </summary>
	std::vector<glm::vec3> vertices;

	/// <summary>
	/// Maximal Curvatures:
	/// x - maximal value
	/// yzw - direction 
	/// </summary>
	std::vector<glm::vec4> max_curvatures;

	/// <summary>
	/// Minimal Curvatures:
	/// x - minimal value
	/// yzw - direction
	/// </summary>
	std::vector<glm::vec4> min_curvatures;

	std::vector<float> min_curvature_values;
	std::vector<float> max_curvature_values;

	std::vector<float> view_curvatures;

	std::vector<glm::vec3> view_curvature_derivatives;

	std::vector<glm::vec3> contours;

	glm::mat4 model_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;

	float total_area = 0.0f;

// Methods
public:
	Model(std::string file_path);
	void compute_view_dependent_curvature(glm::vec3 camera_position);
	void compute_contours(glm::vec3 camera_position);

private:
	std::vector<glm::vec3> compute_normals(std::vector<glm::vec3> vertices, std::vector<glm::uvec3> triangle_indices);
	std::vector<glm::vec3> compute_triangle_normals(std::vector<glm::vec3> vertex_normals, std::vector<glm::uvec3> triangle_indices);
	void compute_curvature();
	void compute_view_curvature_derivative();
	void load_model();

};

