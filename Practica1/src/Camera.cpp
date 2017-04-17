
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, GLfloat sense, GLfloat fov) {
	cameraPos = position;
	cameraFront = glm::normalize(glm::vec3(0.f, 0.f, 0.f) - position);
	cameraUp = glm::cross(cameraFront, glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
	sensitivity = sense;
	YAW = PITCH = 0.f;
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
}
void Camera::MouseMove(float offsetX, float offsetY, float dt) {
	PITCH += offsetY * dt * sensitivity;
	//PITCH = clamp(PITCH, -89.9f, 89.9f);
	//Manual clamping
	if (PITCH < -89.9f) {
		PITCH = -89.9f;
	}
	else if (PITCH > 89.9f) {
		PITCH = 89.9f;
	}

	YAW += offsetX * dt * sensitivity;
	//YAW = std::mod(YAW, 360.f);
	//Manual float clamp
	YAW = (float)((int)(YAW * 100.f) % (360 * 100)) / 100.f;
	//cameraFront = glm::normalize(cameraPos + glm::vec3(cos(YAW * sin(PITCH)), sin(PITCH), sin(YAW * sin(PITCH))));
	//cameraUp = glm::cross(cameraFront, glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
	//std::cout << PITCH << " - " << YAW << std::endl;
}
void Camera::MouseScroll(float offset) {
	FOV += offset;
}
glm::mat4 Camera::LookAt() {
	return glm::lookAt(cameraPos, cameraFront, cameraUp);;
}
GLfloat Camera::GetFOV() {
	return FOV;
}