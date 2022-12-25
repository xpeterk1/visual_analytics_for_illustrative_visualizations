#pragma once
#include <glm/glm.hpp>

class Camera
{

private:

	const float min_elevation = -1.5f;
	const float max_elevation = 1.5f;
	
	const float min_distance = 1.0f;
	
	const float angle_sensitivity = 0.008f;
	const float zoom_sensitivity = 0.003f;
	const float sensitivity = 0.05f;
	
	float angle_direction;
	float distance;
	float angle_elevation;
	
	bool is_camera_rotating = false;
	bool is_camera_zooming = false;

	int last_x_position, last_y_position;

	glm::vec3 eye_position;


public:
	
	/// <summary>
	/// Constructor
	/// </summary>
	Camera(int x, int y, float angle_direction, float angle_elevation, float distance);

	/// <summary>
	/// Mouse movement event handler
	/// </summary>
	/// <param name="x">Current X coordinate</param>
	/// <param name="y">Current Y coordinate</param>
	void on_mouse_move(int x, int y);

	/// <summary>
	/// Mouse button clicked event handler
	/// </summary>
	void on_mouse_button_down(int button, int action);

	/// <summary>
	/// Set the camera position to be the same as the ray
	/// </summary>
	/// <param name="button"></param>
	/// <param name="action"></param>
	void set_camera(glm::vec3 point);

	/// <summary>
	/// Get position of the camera's eye
	/// </summary>
	glm::vec3 get_camera_eye();

private:
	void update_eye();
};

