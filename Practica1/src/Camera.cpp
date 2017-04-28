
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
		cameraPos -= cameraFront * dt * sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos += cameraFront * dt * sensitivity;
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
	PITCH -= offsetY * dt * sensitivity * 50.f;
	PITCH = glm::clamp(PITCH, -89.9f, 89.9f);

	YAW += offsetX * dt * sensitivity * 50.f;
	YAW = glm::mod(YAW, 360.f);

	float frontX = cos(glm::radians(PITCH)) * cos(glm::radians(YAW));
	float frontY = sin(glm::radians(PITCH));
	float frontZ = cos(glm::radians(PITCH)) * sin(glm::radians(YAW));
	cameraFront = glm::normalize(glm::vec3(frontX, frontY, frontZ));
	cameraUp = glm::normalize(glm::cross(cameraFront, glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f))));
}
void Camera::MouseScroll(float offset) {
	FOV += offset;
}
glm::mat4 Camera::LookAt() {
	glm::vec3 cameraRight(glm::normalize(glm::cross(cameraFront, cameraUp)));


	glm::mat4 cameraVariablesMatrix(cameraRight.x, cameraUp.x, cameraFront.x,	0,
									cameraRight.y, cameraUp.y, cameraFront.y,	0,
									cameraRight.z, cameraUp.z, cameraFront.z,	0,
												0,			0,				0,	1);

	glm::mat4 cameraPositionMatrix(1, 0, 0, 0,
								   0, 1, 0, 0,
								   0, 0, 1, 0,
									-cameraPos.x, -cameraPos.y, -cameraPos.z, 1);

	glm::mat4 lookAtMatrix = cameraVariablesMatrix * cameraPositionMatrix;

	return lookAtMatrix;
}
GLfloat Camera::GetFOV() {
	return FOV;
}