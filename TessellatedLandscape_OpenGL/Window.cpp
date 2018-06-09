#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"

//TODO: un bel singleton

Window::Window(int height, int width, const char* title)
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(height, width, title, NULL, NULL);
	ShouldClose = false;

	if (window == NULL) // Moooo: If-else ain't  | A E S T H E T I C | 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	else { // Only if there's a window
		glfwMakeContextCurrent(this->window);
		glfwSetFramebufferSizeCallback(this->window, this->framebuffer_size_callback);
	}
}

Window::~Window()
{
	glfwTerminate();
}

bool Window::isClosed()
{
	return ShouldClose;
}

bool Window::isOk() {
	if (this->window == NULL)
		return false;

	return true;
}

void Window::Close()
{
	ShouldClose = true;
}

GLFWwindow * Window::getWindow()
{
	return window;
}

void Window::processInput()
{
	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		//glfwSetWindowShouldClose(this->window, true);
		this->Close();
	}
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


