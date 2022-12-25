#include "Camera.h"
#include <glfw3.h>

Camera::Camera(int x, int y, float angle_direction, float angle_elevation, float distance) {
	last_x_position = x;
	last_y_position = y;

	this->angle_direction = angle_direction;
	this->angle_elevation = angle_elevation;
	this->distance = distance;

	eye_position = glm::vec3(0.0f);
}

void Camera::on_mouse_move(int x, int y)
{
	float dx = static_cast<float>(x - last_x_position);
	float dy = static_cast<float>(y - last_y_position);
	last_x_position = static_cast<int>(x);
	last_y_position = static_cast<int>(y);

	if (is_camera_rotating) {
		angle_direction += dx * angle_sensitivity;
		angle_elevation += dy * angle_sensitivity;

		// Clamps the results.
		angle_elevation = glm::clamp(angle_elevation, min_elevation, max_elevation);
	}

	if (is_camera_zooming) {
		distance *= (1.0f + dy * zoom_sensitivity);

		// Clamps the results.
		if (distance < min_distance)
			distance = min_distance;
	}

	update_eye();
}

void Camera::update_eye() 
{

	eye_position.x = distance * cosf(angle_elevation) * -sinf(angle_direction);
	eye_position.y = distance * sinf(angle_elevation);
	eye_position.z = distance * cosf(angle_elevation) * cosf(angle_direction);
}

void Camera::on_mouse_button_down(int button, int action)
{
	// Left mouse button affects the angles.
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			is_camera_rotating = true;
		}
		else {
			is_camera_rotating = false;
		}
	}
	// Right mouse button affects the zoom.
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			is_camera_zooming = true;
		}
		else {
			is_camera_zooming = false;
		}
	}
}

void Camera::set_camera(glm::vec3 point) {
	float radius = sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2));

	float theta = std::atan2(point.y, point.x);
	float phi = std::acos(point.z / radius);

	float pitch = std::atan2(point.y, point.x);
	float yaw = std::atan2(point.x, point.z);

	angle_direction = yaw;
	angle_elevation = pitch;

	update_eye();
}

glm::vec3 Camera::get_camera_eye() {
	return eye_position;
}
