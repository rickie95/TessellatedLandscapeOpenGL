#pragma once
#include "Camera.h"

class Window
{
private:
	bool ShouldClose;
	GLFWwindow* window;
	Camera* camera;
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void static mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void setWindow(GLFWwindow* window);

	//void static updateMouse(double xpos, double ypos);
	

public:
	Window(int height, int width, const char* title, Camera* cam);
	~Window();
	bool isClosed();
	bool isOk();
	void Close();
	void setCamera(Camera* cam);
	GLFWwindow* getWindow();
	void processInput();

};

