#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <thread>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Perlin.h"
#include "Object.h"
#include "CustomTypes.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

int main()
{
		
	const int MAX_FPS = 60;

	static Camera* camera = new Camera(glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Window* window = new Window(800, 600, "A E S T H E T I C  L A N D S C A P E", camera);
	window->setCamera(camera);
	if (!window->isOk())
		return -1;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Creating Shaders
	Shader* shader = new Shader("vertex.glsl", "fragment.glsl", "geometry.glsl");

	// HEIGHT MAP

	int h = 512, ww = 512;
	heightMap* hm = createNoiseMap(h, ww, 40.0, 40.0, 7, rand() % 10, 0.52);
	
	Object* terrain = new Object(hm->coords);
	terrain->setIndices(hm->indices);
	terrain->setShader(shader);
	terrain->setDrawMode(GL_TRIANGLES);

	// WATER
	int i = 0;
	std::vector<float3>* wat_verts = new std::vector<float3>(4);
	float3 f = { 20*1.0, 0.0, 20*1.0 };
	(*wat_verts)[i++] = f;
	f = { 20*-1.0, 0.0, 20*1.0 };
	(*wat_verts)[i++] = f;
	f = { 20 * 1.0, 0.0, 20 * -1.0 };
	(*wat_verts)[i++] = f;
	f = { 20 * -1.0, 0.0, 20 * -1.0 };
	(*wat_verts)[i++] = f;
	
	i = 0;
	std::vector<uint3>* wat_ind = new std::vector<uint3>(2);
	uint3 ind = { 0, 2, 1};
	(*wat_ind)[i++] = ind;
	ind = { 1, 2, 3 };
	(*wat_ind)[i++] = ind;


	Shader* water_shader = new Shader("vertex_water.glsl", "fragment_water.glsl");

	Object* water = new Object(wat_verts);
	water->setShader(water_shader);
	water->setIndices(wat_ind);
	water->setDrawMode(GL_TRIANGLES);

	int waitFor = 1 / ((float)MAX_FPS)*1000;
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.5f, 0.5f, -0.5f));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glm::vec3 lightPosition = glm::vec3(5.0f, 15.0f, 5.0f);
	model = glm::mat4(1.0f);
	float x = 0;
	
	Shader* s = NULL;
	while (!window->isClosed())
	{
		
		window->processInput();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightPosition = glm::vec3(5.0f, 5.0f, 5.0f);

		// TERRAIN 
		s = terrain->useShader();
		s->setData("model", model);
		s->setData("projection", camera->getProjection());
		s->setData("view", camera->getLookAt());
		s->setData("lightPos", lightPosition);
		terrain->drawObject();

		//WATER
		s = water->useShader();
		s->setData("model", model);
		s->setData("projection", camera->getProjection());
		s->setData("view", camera->getLookAt());
		water->drawObject();
		
		glfwSwapBuffers(window->getWindow());
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));
		
	}

	terrain->~Object();
	water->~Object();

	window->~Window();

	//system("PAUSE");
	return 0;
}

