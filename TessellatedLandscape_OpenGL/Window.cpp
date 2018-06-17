#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"

//TODO: un bel singleton
unsigned int lastX = 400;
unsigned int lastY = 300;
unsigned int ViewportWidth = 800;
unsigned int ViewportHeight = 600;
bool firstMouse = true;
Camera* camera_off;

Window::Window(int height, int width, const char* title, Camera* cam)
{
	// glfw: initialize and configure
	// ------------------------------
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	camera_off = cam;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	ShouldClose = false;

	window = glfwCreateWindow(height, width, title, NULL, NULL);
	setWindow(window);
}

Window::~Window(){
	glfwTerminate();
}

bool Window::isClosed(){
	return ShouldClose;
}

bool Window::isOk(){
	if (this->window == NULL)
		return false;
	return true;
}

void Window::Close(){
	ShouldClose = true;
}

void Window::setCamera(Camera * cam){
	camera = cam;
	camera_off = cam;
}

GLFWwindow * Window::getWindow(){
	return window;
}

void Window::setWindow(GLFWwindow* window) {
	this->window = window;
	if (window == NULL) // Moooo: If-else ain't  | A E S T H E T I C | 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	else { // Only if there's a window
		glfwMakeContextCurrent(this->window);
		glfwSetFramebufferSizeCallback(this->window, this->framebuffer_size_callback);
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(this->window, this->mouse_callback);
	}
}

void Window::processInput()
{
	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		this->Close();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->moveX(1);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->moveX(-1);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->moveZ(-1);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->moveZ(1);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera->moveY(1);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		camera->moveY(-1);
	}
	if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
		camera->Velocity(1);
	}
	if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
		camera->Velocity(-1);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowSize(this->window, mode->width - 50, mode->height - 50);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowSize(this->window, 800, 600);
	}
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	camera_off->updateAspectRatio(width, height);
}

void MouseMove(double x, double y) {
	
	if (camera_off != NULL) {
		camera_off->updateMouse(x, y);
	}
	
}
void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	MouseMove(xpos, ypos);
	
}


