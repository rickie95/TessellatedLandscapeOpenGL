#pragma once
class Window
{
private:
	bool ShouldClose;
	GLFWwindow* window;
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

public:
	Window(int height, int width, const char* title);
	~Window();
	bool isClosed();
	bool isOk();
	void Close();
	GLFWwindow* getWindow();
	void processInput();
};

