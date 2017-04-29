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
	Shader shader("./src/ObjectVertexShader.vertexshader", "./src/ObjectFragmentShader.fragmentshader");
	//3D cubes info
	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};
	//Vertices Definition
	//Here we initialize the points that will form our shape
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f  // Top Left 
	};
	//EBO definition
	//Here we define which point form a triangle
	int triangles[]{
		0, 3, 1,
		2, 1, 3
	};

	// VBO, VAO and EBO creation
	GLuint VBO; //pointer to the VBO --- VBO = Vertex Buffer Object
	GLuint VAO; //pointer to the VAO --- VAO = Vertice Array Object

	/////////////////// VBO SETUP ////////////////////
	glGenBuffers(1, &VBO); // We generate a buffer to store the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //We bind the VBO to it's buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);

	/////////////////// VAO SETUP ////////////////////	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); 

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	/////////////////// BINDINGS RESET ////////////////////
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/////////////////// TEXTURE LOADING ////////////////////
	//Texture 1
	GLuint T1;
	int T1W, T1H; // We will store the texture width and height in those variables
	LoadTexture(T1, T1W, T1H, "./src/texture.png");

	//Texture 1
	GLuint T2;
	int T2W, T2H; // We will store the texture width and height in those variables
	LoadTexture(T2, T2W, T2H, "./src/planetTexture.png");

	/////////////////// TRANSFORMATION / MODEL  MATRIX ////////////////////
	vec3 positionVec(0.f, -0.5f, 0.f); // New position
	vec3 scaleVec(1.f, 1.f, 1.0f); // New scale
	vec3 rotationVec(1.0f, 0.0f, 0.0f); // plane used to rotate the figure
	float rotation = 0.0f;

	mat4 transformationMatrix(1.0f);
	//Translate -> Rotate -> Scale
	transformationMatrix = glm::translate(transformationMatrix, positionVec);
	transformationMatrix = glm::rotate(transformationMatrix, rotation, rotationVec);
	transformationMatrix = glm::scale(transformationMatrix, scaleVec);

	/////////////////// VIEW MATRIX ////////////////////
	//The "look at" matrix used for the camera


	//Actual look at matrix


	/////////////////// GET THE UNIFORM VARIABLES ////////////////////
	//Now we get the transformation matrix handle from the vertex shader
	//GLint offset = glGetUniformLocation(shader.Program, "offset"); // We save the direction of the variable offset to a pointer
	//GLuint matrixID = glGetUniformLocation(shader.Program, "matrix");
	
	GLuint projMatrixID = glGetUniformLocation(shader.Program, "projection"); // Perspective / Ortho camera
	GLuint viewMatrixID = glGetUniformLocation(shader.Program, "view"); // The "camera" matrix
	GLuint modelMatrixID = glGetUniformLocation(shader.Program, "model"); // IMPORTANT -> model = transformation
	
	float lastFrame = (float)glfwGetTime();
	//DRAW LOOP
	Model spider("C:/Users/Usuario/Documents/spider/spider.obj");

	while (!glfwWindowShouldClose(window)) {
		deltaTime = (float)glfwGetTime() - lastFrame;
		lastFrame = (float)glfwGetTime();

		/////////////////// CLEAR THE COLOR BUFFER AND SET BACKGROUND COLOR ////////////////////
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);*/

		glClearColor(217.f / 255.f, 233.f / 255.f, 1.f, 1.0f);
		glEnable(GL_DEPTH_TEST);

		/////////////////// SHADER USAGE ////////////////////
		shader.USE();

		/////////////////// BIND VAO ////////////////////
		glBindVertexArray(VAO); // We are using the vao attributes here, we "paint" the VAO

		/////////////////// TEXTURE USAGE ////////////////////
		//We bind the first texture to the uniform variable "Texture1" in the fragment shader
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, T1);
		glUniform1i(glGetUniformLocation(shader.Program, "Texture1"), 0);

		//We do the same as before but using another buffer and the second texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, T2);
		glUniform1i(glGetUniformLocation(shader.Program, "Texture2"), 1);*/
		/////////////////// PROJECTION MARIX (CAMERA) ////////////////////

		//Prespective Camera (FOV)
		float AspectRatio = WIDTH / HEIGHT;
		glm::mat4 perspProj = glm::perspective(radians(cam.GetFOV()), AspectRatio, 0.1f, 100.f);

		/////////////////// VIEW MATRIX ////////////////////
		cam.DoMovement(window, deltaTime);
		/*for (int i = 0; i < 10; i++) {
			///////////////////  TRANSFORMATION MATRIX ////////////////////
			//We recalculate the movement with the new values
			transformationMatrix = glm::mat4(1.0f);
			rotationVec = glm::vec3(0.0f, 1.0f, 0.0f);
			//transformationMatrix = glm::rotate(transformationMatrix, (float)glfwGetTime() * 2, rotationVec);
			transformationMatrix = glm::translate(transformationMatrix, CubesPositionBuffer[i]);
			rotationVec = glm::vec3(1.0f, 0.0f, 0.0f);
			transformationMatrix = glm::rotate(transformationMatrix, rotation, rotationVec);
			transformationMatrix = glm::scale(transformationMatrix, scaleVec);
			//We comunicate with glsl to overwrite the matrix it has
			glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(perspProj));
			glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE,glm::value_ptr(cam.LookAt()));
			glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
			rotation = (sin(glfwGetTime()) * 2 * PI);

			/////////////////// PAINT //////////////////// 
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/
		transformationMatrix = glm::mat4(1.0f);
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.1f, -0.1f, 0.1f));

		glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(perspProj));
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(cam.LookAt()));
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

		spider.Draw(shader, GL_FILL);
		/////////////////// INPUT PROCESSING ////////////////////
		glfwPollEvents();
		/////////////////// SWAP SCREEN BUFFERS /////////////////////
		glfwSwapBuffers(window);
	}

	// Free the VAO, VBO and EBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
