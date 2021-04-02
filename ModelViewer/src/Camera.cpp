#include "Camera.h"

const float Camera::YAW			= -90.0f;
const float Camera::PITCH		=  0.0f;
const float Camera::SPEED		=  2.5f;
const float Camera::SENSITIVITY =  0.1f;
const float Camera::ZOOM		=  45.0f;

// constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
		: position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up), 
		yaw(yaw), pitch(pitch), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	updateCameraVectors();
}

// constructor with scalars
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: position(glm::vec3(posX, posY, posZ)), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(glm::vec3(upX, upY, upZ)), 
	yaw(yaw), pitch(pitch), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

float Camera::getZoom()
{
	return zoom;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == CameraMovement::FORWARD)
		position += front * velocity;
	if (direction == CameraMovement::BACKWARD)
		position -= front * velocity;
	if (direction == CameraMovement::LEFT)
		position -= right * velocity;
	if (direction == CameraMovement::RIGHT)
		position += right * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	// make sure that when pitch is out of bounds,
	// screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// update front, right and up vectors
	// using the updated Euler angles
	updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
	zoom -= (float)yOffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

// calculates the front vector
// from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	// calculate the new front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);

	// also re-calculate the right and up vector
	// normalize the vectors, because their length gets closer to 0
	// the more you look up or down which results in slower movement.
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
