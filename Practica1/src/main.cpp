//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include <SOIL.h>
//Shader
#include "shader.h"
#include "Camera.h"
//Matrix and vectors
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Model.h"
#include "Object.h"

#define PI 3.1416f

using namespace std;
using namespace glm;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIREFRAME = false;
float deltaTime = 0.f;

Camera cam(vec3(0.f, 0.f, -3.f), vec3(0.f, 0.f, 0.f), 10.f, 60.f);

float lastMouseX;
float lastMouseY;
bool frameOne = true;

glm::vec3 cameraPosition;
glm::vec3 cameraFront;
glm::vec3 cameraRight;
glm::vec3 cameraUp;

Object* obj;
Object* lightCube;
glm::vec3 boxColor(0.1f, 0.1f, 5.f);
glm::vec3 lightColor(1.f);
glm::vec3 lightPosition(0.f, 0.f, 0.f);


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void LoadTexture(GLuint&, int&, int&, const std::string&);

int main() {
	/////////////////// GLFW INITIALISATION ////////////////////
	//initGLFW
	GLFWwindow * window;

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	/////////////////// GLFW CONFIGURATION ////////////////////
	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	/////////////////// WINDOW CREATION ////////////////////
	//create a window
	window = glfwCreateWindow(WIDTH, HEIGHT, "OPENGL_FRAMEWORK", nullptr, nullptr);

	if (!window) {
		std::cout << "Error al cargar la ventana" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	/////////////////// GLEW INITIALISATION ////////////////////
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Error al cargar glew" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/////////////////// KEY / MOUSE CALLBACK //////////////////////
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	/////////////////// SHADER LOADING ////////////////////
	//Cubes shader
	//Shader shader("./src/Shaders/SpotLightVertexShader.vertexshader", "./src/Shaders/SpotLightFragmentShader.fragmentshader");
	Shader shader("./src/Shaders/FocusLightVertexShader.vertexshader", "./src/Shaders/FocusLightFragmentShader.fragmentshader");
	Shader unlitShader("./src/Shaders/UnlitVertexShader.vertexshader", "./src/Shaders/UnlitFragmentShader.fragmentshader");
	/////////////////// GET THE UNIFORM VARIABLES ////////////////////

	GLuint projMatrixID = glGetUniformLocation(shader.Program, "projection"); // Perspective / Ortho camera
	GLuint viewMatrixID = glGetUniformLocation(shader.Program, "view"); // The "camera" matrix
	GLuint modelMatrixID = glGetUniformLocation(shader.Program, "model"); // IMPORTANT -> model = transformation
	//Light uniforms
	GLuint cubeColorID = glGetUniformLocation(shader.Program, "cubeColor");
	GLuint lightColorID = glGetUniformLocation(shader.Program, "lightColor");
	GLuint lightPositionID = glGetUniformLocation(shader.Program, "lightPosition");
	GLuint directionID = glGetUniformLocation(shader.Program, "direction");
	GLuint outerConeID = glGetUniformLocation(shader.Program, "outerCone");
	GLuint innerConeID = glGetUniformLocation(shader.Program, "innerCone");
	GLuint cameraPositionID = glGetUniformLocation(shader.Program, "cameraPosition");

	GLuint unlitColorID = glGetUniformLocation(unlitShader.Program, "faceColor");
	GLuint unlitProjMatrixID = glGetUniformLocation(unlitShader.Program, "projection"); // Perspective / Ortho camera
	GLuint unlitViewMatrixID = glGetUniformLocation(unlitShader.Program, "view"); // The "camera" matrix
	GLuint unlitModelMatrixID = glGetUniformLocation(unlitShader.Program, "model"); // IMPORTANT -> model = transformation

	float lastFrame = (float)glfwGetTime();
	//DRAW LOOP
	Model spider("./models/spider/spider.obj");
	obj = new Object(glm::vec3(0.f, 0.f, -8.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	lightCube = new Object(lightPosition, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.1f, 0.1f, 0.1f));

	while (!glfwWindowShouldClose(window)) {
		deltaTime = (float)glfwGetTime() - lastFrame;
		lastFrame = (float)glfwGetTime();

		/////////////////// CLEAR THE COLOR BUFFER AND SET BACKGROUND COLOR ////////////////////
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);

		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glEnable(GL_DEPTH_TEST);

		/////////////////// SHADER USAGE ////////////////////
		shader.USE();

		//Prespective Camera (FOV)
		float AspectRatio = WIDTH / HEIGHT;
		glm::mat4 perspProj = glm::perspective(radians(cam.GetFOV()), AspectRatio, 0.1f, 100.f);

		/////////////////// VIEW MATRIX ////////////////////
		cam.DoMovement(window, deltaTime);

		glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(perspProj));
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(cam.LookAt()));
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(obj->GetModelMatrix()));

		glUniform3f(cubeColorID, boxColor.x, boxColor.y, boxColor.z);
		glUniform3f(lightColorID, lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(lightPositionID, lightPosition.x, lightPosition.y, lightPosition.z);
		glUniform3f(cameraPositionID, cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
		glUniform3f(directionID, 0.f, 0.f, 1.f);
		glUniform1f(innerConeID, glm::radians(60.f));
		glUniform1f(outerConeID, glm::radians(90.f));
		//spider.Draw(shader, GL_FILL);
		obj->Draw();

		unlitShader.USE();
		glUniformMatrix4fv(unlitProjMatrixID, 1, GL_FALSE, glm::value_ptr(perspProj));
		glUniformMatrix4fv(unlitViewMatrixID, 1, GL_FALSE, glm::value_ptr(cam.LookAt()));
		glUniformMatrix4fv(unlitModelMatrixID, 1, GL_FALSE, glm::value_ptr(lightCube->GetModelMatrix()));

		glUniform3f(unlitColorID, lightColor.x, lightColor.y, lightColor.z);
		lightCube->Draw();
		/////////////////// INPUT PROCESSING ////////////////////
		glfwPollEvents();
		/////////////////// SWAP SCREEN BUFFERS /////////////////////
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		obj->Move(obj->GetPosition() + glm::vec3(0.f, 1.f, 0.f));
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		obj->Move(obj->GetPosition() - glm::vec3(0.f, 1.f, 0.f));
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		obj->Move(obj->GetPosition() + glm::vec3(1.f, 0.f, 0.f));
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		obj->Move(obj->GetPosition() - glm::vec3(1.f, 0.f, 0.f));
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(10.f, 0.f, 0.f));
	}
	if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(-10.f, 0.f, 0.f));
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(0.f, 0.f, 10.f));
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(0.f, 0.f, -10.f));
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (frameOne) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		frameOne = false;
	}
	else {
		cam.MouseMove(xpos - lastMouseX, ypos - lastMouseY, deltaTime);
		lastMouseX = xpos;
		lastMouseY = ypos;
	}

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	cam.MouseScroll(yoffset);
}
void LoadTexture(GLuint& pointer, int& width, int& height, const std::string& path) {
	glGenTextures(1, &pointer); // We generate the actual texture
	glBindTexture(GL_TEXTURE_2D, pointer); // WE bind it to the graphics card
										   // Set the texture wrapping/filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Load and generate the texture
	unsigned char* myTexture = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, myTexture); // We generate the texture
	SOIL_free_image_data(myTexture); // We free the image since we have stored it
	glBindTexture(GL_TEXTURE_2D, 0); // We bind the texture to the graphics card
}
void CreateModelMatrix(glm::vec3 position, glm::vec3 rotationAxis, float rotation, glm::vec3 scale) {
	//We recalculate the movement with the new values
	glm::mat4 newMatrix;
	newMatrix = glm::mat4(1.0f);
	newMatrix = glm::translate(newMatrix, position);
	newMatrix = glm::rotate(newMatrix, rotation, rotationAxis);
	newMatrix = glm::scale(newMatrix, scale);
}