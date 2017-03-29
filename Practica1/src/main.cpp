//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
//Shader
#include "shader.h"

using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIREFRAME = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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
	/////////////////// SHADER LOADING ////////////////////
	Shader shader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	//Vertices Definition
	//Here we initialize the points that will form our shape
	float vertices []{
		-0.2f, -0.2f, 0.f,
		0.2f, -0.2f, 0.f,
		0.2f, 0.2f, 0.f,
		-0.2f, 0.2f, 0.f
	};
	//EBO definition
	//Here we define which point form a triangle
	int triangles[]{
		0, 1, 3,
		1, 2, 3
	};

	// VBO, VAO and EBO creation
	GLuint VBO; //pointer to the VBO --- VBO = Vertex Buffer Object
	GLuint EBO; //pointer to the EBO --- EBO = Element Buffer Object
	GLuint VAO; //pointer to the VAO --- VAO = Vertice Array Object

	/////////////////// VBO SETUP ////////////////////
	glGenBuffers(1, &VBO); // We generate a buffer to store the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //We bind the VBO to it's buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //We allocate enough memory to hold all our vertices
	//We now have the VBO allocated in memory, binded with it's buffer and filled with the necessary information

	/////////////////// EBO SETUP ////////////////////
	glGenBuffers(1, &EBO); // We generate a buffer to store the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //We bind the VBO to it's buffer, in this case an element array one
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW); //We allocate enough memory to hold all the elements that we will draw

	/////////////////// VAO SETUP ////////////////////
	glGenVertexArrays(1, &VAO);//We create a vertex array buffer that will store the VAO properties
	glBindVertexArray(VAO); // We bind that VAO to our pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (GLvoid*)0);//Here we store enough memory for a vao taht will work with 3 dimensional vertices

	//We reset the bindings
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//DRAW LOOP
	while (!glfwWindowShouldClose(window)) {

		/////////////////// CLEAR THE COLOR BUFFER AND SET BACKGROUND COLOR ////////////////////
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.2f, 0.0f, 1.0f);

		/////////////////// SHADER USAGE ////////////////////
		shader.USE();

		//pitar el VAO
		glBindVertexArray(VAO); // We are using the vao attributes here, we "paint" the VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		if (!WIREFRAME) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);

		/////////////////// SWAP SCREEN BUFFERS /////////////////////
		glfwSwapBuffers(window);

		/////////////////// INPUT PROCESSING ////////////////////
		glfwPollEvents();

	}

	// Free the VAO, VBO and EBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		WIREFRAME = !WIREFRAME;
	}
}
