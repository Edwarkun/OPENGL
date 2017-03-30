//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include <SOIL.h>
//Shader
#include "shader.h"
//Matrix and vectors
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


using namespace std;
using namespace glm;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIREFRAME = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
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
	/////////////////// SHADER LOADING ////////////////////
	Shader shader("./src/TextureVertexShader.vertexshader", "./src/TextureFragmentShader.fragmentshader");

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
	
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (GLvoid*)0);//Here we store enough memory for a vao taht will work with 3 dimensional vertices
	glEnableVertexAttribArray(0);//We bind that layout to the shader
	
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (GLvoid*)(3 * sizeof(GLfloat)));//Storage for the color coordinates
	glEnableVertexAttribArray(1);//We bind that layout to the shader
	
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (GLvoid*)(6 * sizeof(GLfloat)));//Storage for the texture normals
	glEnableVertexAttribArray(2);//We bind that layout to the shader

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

	/////////////////// TRANSFORMATION MATRIX ////////////////////
	vec3 positionVec(0.5f, 0.5f, 0.f);
	vec3 scaleVec(0.5f, -0.5f, 1.0f);
	vec3 rotationVec(0.0f, 0.0f, 1.0f);

	mat4x4 transformationMatrix;
	// Translate -> Rotate -> Scale
	transformationMatrix = glm::translate(transformationMatrix, positionVec);
	transformationMatrix = glm::rotate(transformationMatrix, 0.0f, rotationVec);
	transformationMatrix = glm::scale(transformationMatrix, scaleVec);

	//DRAW LOOP
	while (!glfwWindowShouldClose(window)) {

		/////////////////// CLEAR THE COLOR BUFFER AND SET BACKGROUND COLOR ////////////////////
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.2f, 0.0f, 1.0f);

		/////////////////// SHADER USAGE ////////////////////
		shader.USE();

		/////////////////// UNIFORM VARIABLES TWEAKING ////////////////////
		/*GLint offset = glGetUniformLocation(shader.Program, "offset"); // We save the direction of the variable offset to a pointer
		glUniform1f(offset, abs(sin(glfwGetTime()) * 0.2f));*/
		GLint texClamp = glGetUniformLocation(shader.Program, "textureClamp");
		glUniform1f(texClamp, (sin(glfwGetTime()) + 1) / 2);

		///////////////////  TRANSFORMATION MATRIX ////////////////////
		glUniformMatrix4fv(shader.Program, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

		/////////////////// BIND VAO AND EVO ////////////////////
		glBindVertexArray(VAO); // We are using the vao attributes here, we "paint" the VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // WE define the EBO we're using

		/////////////////// TEXTURE USAGE ////////////////////
		//We bind the first texture to the uniform variable "Texture1" in the fragment shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, T1);
		glUniform1i(glGetUniformLocation(shader.Program, "Texture1"), 0);

		//We do the same as before but using another buffer and the second texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, T2);
		glUniform1i(glGetUniformLocation(shader.Program, "Texture2"), 1);

		/////////////////// WIREFRAME / FILL & PAINT ////////////////////
		if (!WIREFRAME) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0); // Reset VAO
		/////////////////// INPUT PROCESSING ////////////////////
		glfwPollEvents();
		/////////////////// SWAP SCREEN BUFFERS /////////////////////
		glfwSwapBuffers(window);
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
void LoadTexture(GLuint& pointer, int& width, int& height, const std::string& path) {
	glGenTextures(1, &pointer); // We generate the actual texture
	glBindTexture(GL_TEXTURE_2D, pointer); // WE bind it to the graphics card
										   // Set the texture wrapping/filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	unsigned char* myTexture = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, myTexture); // We generate the texture
	SOIL_free_image_data(myTexture); // We free the image since we have stored it
	glBindTexture(GL_TEXTURE_2D, 0); // We bind the texture to the graphics card
}