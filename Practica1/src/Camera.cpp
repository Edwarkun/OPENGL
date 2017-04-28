
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, GLfloat sense, GLfloat fov) {
	cameraPos = position;
	cameraFront = glm::normalize(glm::vec3(0.f, 0.f, 0.f) - position);
	cameraUp = glm::cross(cameraFront, glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
	sensitivity = sense;
	YAW = 90.f;
	PITCH = 0.f;
	FOV = fov;
}

void Camera::DoMovement(GLFWwindow* window, float dt) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraFront * dt * sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraFront * dt * sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::cross(cameraFront, cameraUp) * dt * sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::cross(cameraFront, cameraUp) * dt * sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cameraPos += cameraUp * dt * sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cameraPos -= cameraUp * dt * sensitivity;
	}
}
void Camera::MouseMove(float offsetX, float offsetY, float dt) {
	PITCH += offsetY * dt * sensitivity * 100;
	PITCH = glm::clamp(PITCH, -89.9f, 89.9f);

	YAW -= offsetX * dt * sensitivity * 100;
	YAW = glm::mod(YAW, 360.f);

	float frontX = cos(glm::radians(PITCH)) * cos(glm::radians(YAW));
	float frontY = sin(glm::radians(PITCH));
	float frontZ = cos(glm::radians(PITCH)) * sin(glm::radians(YAW));
	cameraFront = glm::normalize(glm::vec3(frontX, frontY, frontZ));
	cameraUp = glm::cross(cameraFront, glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
}
void Camera::MouseScroll(float offset) {
	FOV += offset;
}
glm::mat4 Camera::LookAt() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
GLfloat Camera::GetFOV() {
	return FOV;
}