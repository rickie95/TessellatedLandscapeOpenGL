#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <thread>
#include "Shader.h"
#include "Window.h"
#include "Camera.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

int main()
{
	const int MAX_FPS = 60;
	Window* window = new Window(800, 600, "A E S T H E T I C  L A N D S C A P E");
	Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	window->setCamera(camera);

	if (!window->isOk())
		return -1;

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	// Creating Shaders
	Shader* shader = new Shader("vertex.glsl", "fragment.glsl");
	

	// VERTEX DATA
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// RECTANGLE
		0.5f, 0.5f, 0.5f,	1.0f, 0.0f, 0.0f,//top right
		0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,// bottom right
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // bottom left
		-0.5f,  0.5f, 0.5f,  1.0f, 1.0f, 0.0f,// top left

		//REAR
		0.5f, 0.5f, -0.5f,	1.0f, 0.0f, 0.0f,//top right
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,// bottom right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom left
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f// top left
		
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
		0, 5, 1,
		0, 4, 5,
		0, 3, 7,
		0, 7, 4,
		6, 4, 7,
		4, 5, 6,
		1, 5, 6,
		1, 2, 6,
		2, 3, 6,
		3, 6, 7
	};

	

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// now EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);

	// bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can 
	// just bind it beforehand before rendering the respective triangle; this is another approach.
	glBindVertexArray(VAO);

	// render loop
	// -----------
	int waitFor = 1 / ((float)MAX_FPS)*1000;
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glEnable(GL_DEPTH_TEST);

	while (!window->isClosed())
	{
		window->processInput();

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate the shader before any calls to glUniform
		shader->use();
		float timeValue = glfwGetTime();
		//model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		shader->setMat4("projection", projection);
		shader->setMat4("model", model);
		shader->setMat4("view", camera->getLookAt());
		// render the triangle
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window->getWindow());
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));
		

		//window->Close();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	window->~Window();

	//system("PAUSE");
	return 0;
}