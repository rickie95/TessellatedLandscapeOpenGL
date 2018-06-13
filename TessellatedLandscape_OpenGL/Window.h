#pragma once
#include "Camera.h"

class Window
{
private:
	bool ShouldClose;
	GLFWwindow* window;
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	Camera* camera;

public:
	Window(int height, int width, const char* title);
	~Window();
	bool isClosed();
	bool isOk();
	void Close();
	void setCamera(Camera* cam);
	GLFWwindow* getWindow();
	void processInput();
};

