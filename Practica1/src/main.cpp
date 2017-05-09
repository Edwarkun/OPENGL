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
#include "material.h"
#include "Light.h"

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
Material mat("./Materials/difuso.png", "./Materials/especular.png", 64.f);


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
	Shader lightShader("./src/Shaders/VertexShaderPhongTexture.vertexshader", "./src/Shaders/FragmentShaderPhongTexture.fragmentshader");
	/////////////////// LIGHTS + VISUAL CUBES////////////////////
	//Each light has a cube to represent its position and color
	//Directional light
	Light directionalLight(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f), 0.4f, 1.f, 1.f, Light::DIRECTIONAL, 0, glm::vec3(1.f, 1.f, 1.f)); // White

	//2 point lights
	Light pointLight1(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.05f, 1.f, 1.f, Light::POINT, 0, glm::vec3(0.f, 1.f, 1.f)); // Cyan
	Object pointLight1Cube(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.1f, 0.1f, 0.1f));

	Light pointLight2(glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, 0.f, 0.f), 0.05f, 1.f, 1.f, Light::POINT, 1, glm::vec3(0.f, 1.f, 0.f)); // Green
	Object pointLight2Cube(glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.1f, 0.1f, 0.1f));

	//2 spot lights
	Light spotLight1(glm::vec3(0.f, 2.f, 5.f), glm::vec3(0.f, 0.f, -1.f), 0.05f, 1.f, 1.f, Light::SPOT, 0, glm::vec3(1.f, 1.f, 1.f)); // White
	Object spotLight1Cube(glm::vec3(0.f, 2.f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.1f, 0.1f, 0.1f));

	Light spotLight2(glm::vec3(2.f, 0.f, 2.f), glm::vec3(-1.f, 0.f, 0.f), 0.05f, 1.f, 1.f, Light::SPOT, 1, glm::vec3(1.f, 0.f, 1.f)); // Magenta
	Object spotLight2Cube(glm::vec3(2.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.1f, 0.1f, 0.1f));

	/////////////////// GET THE UNIFORM VARIABLES ////////////////////

	GLuint projMatrixID = glGetUniformLocation(lightShader.Program, "projection"); // Perspective / Ortho camera
	GLuint viewMatrixID = glGetUniformLocation(lightShader.Program, "view"); // The "camera" matrix
	GLuint modelMatrixID = glGetUniformLocation(lightShader.Program, "model"); // IMPORTANT -> model = transformation
	
	//Light uniforms
	GLuint unlitColorID = glGetUniformLocation(unlitShader.Program, "faceColor");
	GLuint unlitProjMatrixID = glGetUniformLocation(unlitShader.Program, "projection"); // Perspective / Ortho camera
	GLuint unlitViewMatrixID = glGetUniformLocation(unlitShader.Program, "view"); // The "camera" matrix
	GLuint unlitModelMatrixID = glGetUniformLocation(unlitShader.Program, "model"); // IMPORTANT -> model = transformation

	float lastFrame = (float)glfwGetTime();
	//DRAW LOOP
	//Model spider("./models/spider/spider.obj");
	obj = new Object(glm::vec3(0.f, 0.f, -8.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f));

	mat.SetShininess(&lightShader);
	mat.ActivateTextures();
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
		lightShader.USE();

		//Prespective Camera (FOV)
		float AspectRatio = WIDTH / HEIGHT;
		glm::mat4 perspProj = glm::perspective(radians(cam.GetFOV()), AspectRatio, 0.1f, 100.f);

		/////////////////// VIEW MATRIX ////////////////////
		cam.DoMovement(window, deltaTime);

		glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(perspProj));
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(cam.LookAt()));
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(obj->GetModelMatrix()));

		mat.SetMaterial(&lightShader);
		mat.SetShininess(&lightShader);
		//directionalLight.SetLight(&lightShader, cam.GetPosition());

		spotLight1.SetLight(&lightShader, cam.GetPosition());
		spotLight2.SetLight(&lightShader, cam.GetPosition());

		pointLight1.SetLight(&lightShader, cam.GetPosition());
		pointLight2.SetLight(&lightShader, cam.GetPosition());

		obj->Draw();

		unlitShader.USE();
		glUniformMatrix4fv(unlitProjMatrixID, 1, GL_FALSE, glm::value_ptr(perspProj));
		glUniformMatrix4fv(unlitViewMatrixID, 1, GL_FALSE, glm::value_ptr(cam.LookAt()));

		glUniformMatrix4fv(unlitModelMatrixID, 1, GL_FALSE, glm::value_ptr(spotLight1Cube.GetModelMatrix()));
		glUniform3f(unlitColorID, spotLight1.GetColor().x, spotLight1.GetColor().y, spotLight1.GetColor().z);
		spotLight1Cube.Draw();

		glUniformMatrix4fv(unlitModelMatrixID, 1, GL_FALSE, glm::value_ptr(spotLight2Cube.GetModelMatrix()));
		glUniform3f(unlitColorID, spotLight2.GetColor().x, spotLight2.GetColor().y, spotLight2.GetColor().z);
		spotLight2Cube.Draw();

		glUniformMatrix4fv(unlitModelMatrixID, 1, GL_FALSE, glm::value_ptr(pointLight1Cube.GetModelMatrix()));
		glUniform3f(unlitColorID, pointLight1.GetColor().x, pointLight1.GetColor().y, pointLight1.GetColor().z);
		pointLight1Cube.Draw();

		glUniformMatrix4fv(unlitModelMatrixID, 1, GL_FALSE, glm::value_ptr(pointLight2Cube.GetModelMatrix()));
		glUniform3f(unlitColorID, pointLight2.GetColor().x, pointLight2.GetColor().y, pointLight2.GetColor().z);
		pointLight2Cube.Draw();

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
		obj->Move(obj->GetPosition() - glm::vec3(0.f, 1.f, 0.f));
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		obj->Move(obj->GetPosition() + glm::vec3(0.f, 1.f, 0.f));
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		obj->Move(obj->GetPosition() + glm::vec3(0.f, 0.f, -1.f));
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		obj->Move(obj->GetPosition() - glm::vec3(0.f, 0.f, -1.f));
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		obj->Move(obj->GetPosition() + glm::vec3(1.f, 0.f, 0.f));
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		obj->Move(obj->GetPosition() - glm::vec3(1.f, 0.f, 0.f));
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(10.f, 0.f, 0.f));
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(-10.f, 0.f, 0.f));
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(0.f, 0.f, 10.f));
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(0.f, 0.f, -10.f));
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(0.f, 10.f, 0.f));
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
	obj->Rotate(glm::vec3(0.f, -10.f, 0.f));
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