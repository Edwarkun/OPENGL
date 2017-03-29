//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include <shader.hpp>

using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;

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
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hola mundo", nullptr, nullptr);

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
	//cargamos los shader
	GLuint programID = LoadShaders("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	// Definir el buffer de vertices

	// Definir el EBO

	// Crear los VBO, VAO y EBO
	

	//reservar memoria para el VAO, VBO y EBO


	//Establecer el objeto
		//Declarar el VBO y el EBO

		//Enlazar el buffer con openGL

		//Establecer las propiedades de los vertices

		//liberar el buffer
	
	//liberar el buffer de vertices


	//bucle de dibujado

	while (!glfwWindowShouldClose(window)) {

		/////////////////// INPUT PROCESSING ////////////////////
		glfwPollEvents();

		/////////////////// CLEAR THE COLOR BUFFER AND SET BACKGROUND COLOR ////////////////////
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.2f, 0.0f, 1.0f);

		/////////////////// SHADER USAGE ////////////////////
		glUseProgram(programID);

		//pitar el VAO

		//pintar con lineas
		//pintar con triangulos

		// Swap the screen buffers

		glfwSwapBuffers(window);

	}

	// liberar la memoria de los VAO, EBO y VBO

	// Terminate GLFW, clearing any resources allocated by GLFW.

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_KEY_DOWN) {
		glfwWindowShouldClose(window);
	}
}
