#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 direction, GLfloat sense, GLfloat fov);
	void DoMovement(GLFWwindow * window, float dt);
	void MouseMove(float offsetX, float offsetY, float dt);
	void MouseScroll(float offset);
	glm::mat4 LookAt();
	float GetFOV();
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float sensitivity;
	float PITCH;
	float YAW;
	float FOV;
};