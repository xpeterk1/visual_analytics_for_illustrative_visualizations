#include "Model.h"
#include "Edge.h"

Model::Model(std::string file_path)
{
	this->file_path = file_path;
	load_model();
}

void Model::load_model() {
	tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_path.c_str());

	for (const auto& shape : shapes) {
		for (const auto index : shape.mesh.indices) {
			indices.push_back(index.vertex_index);
		}
	}

	for (int i = 0; i < indices.size(); i += 3) {
		triangle_indices.push_back(glm::uvec3(indices[i], indices[i + 1], indices[i + 2]));
	}

	for (int i = 0; i < attrib.vertices.size(); i += 3) {
		vertices.push_back(glm::vec3(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]));
	}

	normals = compute_normals(vertices, triangle_indices);

	for (int i = 0; i < attrib.vertices.size(); i += 3) {
		vertex_attributes.push_back(glm::vec3(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]));
	}

	for (const auto& normal : normals) {
		vertex_attributes.push_back(normal);
	}

	this->triangle_normals = compute_triangle_normals(normals, triangle_indices);

	n_vertices = attrib.vertices.size() / 3;
	n_indices = indices.size();
	n_triangles = triangle_normals.size();

	//compute the total area of the model
	float total = 0.0f;

	for (const auto& triangle : triangle_indices)
	{
		glm::vec3 p[] =
		{
			vertices[triangle.x],
			vertices[triangle.y],
			vertices[triangle.z]
		};

		auto n = glm::cross(glm::vec3(p[1] - p[0]), glm::vec3(p[2] - p[0]));
		total += glm::length(n) / 2.0f;
	}

	total_area = total;

	compute_curvature();
}

struct Vec3KeyFuncs
{
	size_t operator()(const glm::vec3& k)const
	{
		return std::hash<int>()(*reinterpret_cast<const int*>(&k.x)) ^
			std::hash<int>()(*reinterpret_cast<const int*>(&k.y)) ^
			std::hash<int>()(*reinterpret_cast<const int*>(&k.z));
	}

	bool operator()(const glm::vec3& a, const glm::vec3& b)const
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

std::vector<glm::vec3> Model::compute_normals(std::vector<glm::vec3> vertices, std::vector<glm::uvec3> triangle_indices) {
	std::unordered_map<glm::vec3, glm::vec3, Vec3KeyFuncs> accumulated_normals;

	for (const auto& triangle : triangle_indices) {

		glm::vec3 a = vertices[triangle.x];
		glm::vec3 b = vertices[triangle.y];
		glm::vec3 c = vertices[triangle.z];

		glm::vec3 fst = b - a;
		glm::vec3 snd = c - a;

		glm::vec3 normal = glm::cross(fst, snd);

		for (const auto& vertex : { a, b, c }) {
			if (!accumulated_normals.contains(vertex)) {
				accumulated_normals[vertex] = normal;
			}
			else {
				accumulated_normals[vertex] += normal;
			}
		}
	}

	std::vector<glm::vec3> normals;
	for (const auto& vertex : vertices) {
		normals.push_back(glm::normalize(accumulated_normals[vertex]));
	}

	return normals;
}

std::vector<glm::vec3> Model::compute_triangle_normals(std::vector<glm::vec3> vertex_normals, std::vector<glm::uvec3> triangle_indices)
{
	auto output = std::vector<glm::vec3>();
	for (const auto& triangle : triangle_indices)
	{
		output.push_back(vertex_normals[triangle.x] + vertex_normals[triangle.y] + vertex_normals[triangle.z] / 3.0f);
	}

	return output;
}

struct EdgeKeyFuncs
{
	size_t operator()(const Edge& k)const
	{
		return std::hash<int>()(*reinterpret_cast<const int*>(&k.p)) ^
			std::hash<int>()(*reinterpret_cast<const int*>(&k.q));
	}

	bool operator()(const Edge& a, const Edge& b)const
	{
		return a.endpoints_eq(b);
	}
};

void handle_edge(glm::vec3& v1, glm::vec3& v2, std::unordered_map<glm::vec3, std::vector<Edge>, Vec3KeyFuncs>& edges, float area)
{
	Edge e = Edge(v1, v2);
	e.add_area(area);

	if (edges.contains(v1)) {
		bool found = false;
		for (auto& edge : edges[v1]) {
			if (edge.endpoints_eq(e)) {
				edge.add_area(area);
				found = true;
			}
		}

		if (!found) {
			edges[v1].push_back(e);
		}
	}
	else
	{
		edges.insert({ v1, {e} });
	}

	if (edges.contains(v2)) {
		bool found = false;
		for (auto& edge : edges[v2]) {
			if (edge.endpoints_eq(e)) {
				edge.add_area(area);
				found = true;
			}
		}

		if (!found) {
			edges[v2].push_back(e);
		}
	}
	else
	{
		edges.insert({ v2, {e} });
	}
}

void Model::compute_curvature()
{
	std::unordered_map<glm::vec3, std::vector<Edge>, Vec3KeyFuncs> edges;

	for (const auto& triangle : triangle_indices)
	{
		glm::vec3 a = vertices[triangle.x];
		glm::vec3 b = vertices[triangle.y];
		glm::vec3 c = vertices[triangle.z];

		glm::vec3 fst = b - a;
		glm::vec3 snd = c - a;

		glm::vec3 normal = glm::cross(fst, snd);
		float area = glm::length(normal) / 2.0f;

		handle_edge(a, b, edges, area);
		handle_edge(b, c, edges, area);
		handle_edge(c, a, edges, area);
	}

	for (int i = 0; i < n_vertices; i++)
	{
		glm::vec3 vertex = vertices[i];
		glm::vec3 normal = normals[i];
		Eigen::Vector3d N(normal[0], normal[1], normal[2]);

		//total area
		double total_area = 0.0f;
		for (auto& edge : edges[vertex]) {
			total_area += edge.get_areas();
		}
		total_area /= 2.0f;

		Eigen::Matrix3d M = Eigen::Matrix3d::Zero();
		for (auto& edge : edges[vertex])
		{
			// Compute T_ij
			Eigen::Matrix3d IN = Eigen::Matrix3d::Identity(3, 3) - N * N.transpose();
			glm::vec3 pj = edge.get_second_point(vertex);
			glm::vec3 diffv = pj - vertex;
			Eigen::Vector3d DiffV(diffv[0], diffv[1], diffv[2]);
			Eigen::Vector3d T = IN * DiffV;
			T.normalize();

			// Compute k_ij (curvature)
			double normDiffV = DiffV.norm();
			double k = 2.0 * N.transpose() * DiffV;
			k /= (normDiffV * normDiffV);

			// Compute w_ij
			double area = edge.get_areas();
			double w = area / (2.0 * total_area);

			M += w * k * T * T.transpose();
		}

		// Solve eigenvalues
		Eigen::EigenSolver<Eigen::Matrix3d> solver(M);
		Eigen::Matrix3d eigenvectors = solver.pseudoEigenvectors();
		Eigen::Vector3cd eigenvalues = solver.eigenvalues();

		Eigen::Vector3d v1 = eigenvectors.block(0, 0, 3, 1);
		Eigen::Vector3d v2 = eigenvectors.block(0, 1, 3, 1);
		Eigen::Vector3d v3 = eigenvectors.block(0, 2, 3, 1);
		double eig1 = real(eigenvalues(0));
		double eig2 = real(eigenvalues(1));
		double eig3 = real(eigenvalues(2));

		// Find max, min, and 0 eigenvalues
		double m11, m22;
		Eigen::Vector3d maxv, minv;
		double thresh = 0.01;
		if (v1.cross(N).norm() < thresh) {
			if (eig2 < eig3) { m11 = eig2; m22 = eig3; maxv = v2; minv = v3; }
			else { m11 = eig3; m22 = eig2; maxv = v3; minv = v2; }
		}
		else if (v2.cross(N).norm() < thresh) {
			if (eig3 < eig1) { m11 = eig3; m22 = eig1; maxv = v3; minv = v1; }
			else { m11 = eig1; m22 = eig3; maxv = v1; minv = v3; }
		}
		else if (v3.cross(N).norm() < thresh) {
			if (eig1 < eig2) { m11 = eig1; m22 = eig2; maxv = v1; minv = v2; }
			else { m11 = eig2; m22 = eig1; maxv = v2; minv = v1; }
		}
		maxv.normalize();
		minv.normalize();

		max_curvatures.push_back(glm::vec4(m22 - 3 * m11, maxv(0), maxv(1), maxv(2)));
		min_curvatures.push_back(glm::vec4(m11 - 3 * m22, minv(0), minv(1), minv(2)));

		max_curvature_values.push_back(m22 - 3 * m11);
		min_curvature_values.push_back(m11 - 3 * m22);
	}
}

void Model::compute_view_dependent_curvature(glm::vec3 camera_position)
{
	std::vector<float> view_curvatures;
	for (int i = 0; i < n_vertices; i++)
	{
		glm::vec3 p = vertices[i];
		glm::vec3 n = normals[i];
		glm::vec3 v = camera_position - p;

		glm::vec4 max_curvature = max_curvatures[i];
		glm::vec4 min_curvature = min_curvatures[i];

		float dist = glm::dot(n, v);
		glm::vec3 ortho = n * dist;
		glm::vec3 w = v - ortho;
		w = glm::normalize(w);

		glm::vec3 T1 = glm::vec3(max_curvature.y, max_curvature.z, max_curvature.w);
		glm::vec3 T2 = glm::vec3(min_curvature.y, min_curvature.z, min_curvature.w);
		float k1 = max_curvature.x;
		float k2 = min_curvature.x;

		float kw = k1 * glm::dot(w, T1) * glm::dot(w, T1) + k2 * glm::dot(w, T2) * glm::dot(w, T2);

		view_curvatures.push_back(kw);
	}

	this->view_curvatures = view_curvatures;

	compute_view_curvature_derivative();
}

void Model::compute_view_curvature_derivative()
{
	std::vector<glm::vec3> derivatives;
	for (int i = 0; i < n_triangles; i++)
	{
		glm::uvec3 triangle = triangle_indices[i];

		float c[] = {
			view_curvatures[triangle.x],
			view_curvatures[triangle.y],
			view_curvatures[triangle.z]
		};

		glm::vec3 p[] =
		{
			vertices[triangle.x],
			vertices[triangle.y],
			vertices[triangle.z]
		};

		auto n = glm::cross(glm::vec3(p[1] - p[0]), glm::vec3(p[2] - p[0]));
		float area = glm::length(n) / 2.0f;

		n = glm::normalize(n);

		//TODO: zatim davam lokalni normalu, mozna bude pozdeji lepsi tam dat triangle_normal;
		//n = triangle_normals[i];
		glm::vec3 view_curvature_derivative =
			glm::cross(n, (p[0] - p[2])) * (c[1] - c[0]) / (2 * area) +
			glm::cross(n, (p[1] - p[0])) * (c[2] - c[0]) / (2 * area);
		derivatives.push_back(view_curvature_derivative);
	}

	view_curvature_derivatives = derivatives;
}

void Model::compute_contours(glm::vec3 camera_position)
{
	contours.clear();
	for (int i = 0; i < n_triangles; i++)
	{
		glm::vec3 face_normal = glm::normalize(triangle_normals[i]);
		glm::vec3 face_view_curvature_derivative = view_curvature_derivatives[i];

		glm::uvec3 triangle = triangle_indices[i];
		glm::vec3 a = vertices[triangle.x];
		glm::vec3 b = vertices[triangle.y];
		glm::vec3 c = vertices[triangle.z];

		float curv_a = view_curvatures[triangle.x];
		float curv_b = view_curvatures[triangle.y];
		float curv_c = view_curvatures[triangle.z];

		float t;
		bool hasone = false, hastwo = false;
		glm::vec3 p1, p2;

		if (curv_a * curv_b < 0) {
			t = curv_a / (curv_a - curv_b);
			p1 = a + (b - a) * t;
			//h1 = heh_1;
			hasone = true;
		}

		if (curv_b * curv_c < 0) {
			t = curv_b / (curv_b - curv_c);
			if (!hasone) {
				p1 = b + (c - b) * t;
				//h1 = heh_2;
			}
			else {
				p2 = b + (c - b) * t;
				//h2 = heh_2;
				hastwo = true;
			}
			hasone = true;
		}

		if (curv_c * curv_a < 0) {
			t = curv_c / (curv_c - curv_a);
			p2 = c + (a - c) * t;
			//h2 = heh_3;
			hastwo = true;
		}

		if (!hastwo) continue;

		glm::vec3 bary = (a + b + c) / 3.0f;
		glm::vec3 v = camera_position - bary;
		double cos_theta = glm::dot(face_normal, v) / glm::length(v);
		if (cos_theta > cos(suggestive_angle_thresh * _PI / 180)) continue;

		glm::vec3 w1 = glm::normalize(camera_position - p1);
		glm::vec3 w2 = glm::normalize(camera_position - p2);
		double d1 = glm::dot(face_view_curvature_derivative, w1);
		double d2 = glm::dot(face_view_curvature_derivative, w2);

		if (d1 <= suggestive_diff_thresh && d2 <= suggestive_diff_thresh) continue;

		if (d1 > suggestive_diff_thresh && d2 <= suggestive_diff_thresh) {
			t = (d1 - suggestive_diff_thresh) / (d1 - d2);
			p2 = p1 + (p2 - p1) * t;
		}
		else if (d1 <= suggestive_diff_thresh && d2 > suggestive_diff_thresh) {
			t = (d2 - suggestive_diff_thresh) / (d2 - d1);
			p1 = p2 + (p1 - p2) * t;
		}

		contours.push_back(p1);
		contours.push_back(p2);
	}

	for (auto& point : contours) 
	{
		glm::vec3 view_vector = glm::normalize(camera_position - point);
		point = point + 0.1f * view_vector;
	}
}