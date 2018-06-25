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
#include "HeightMap.h"
#include "Object.h"
#include "CustomTypes.h"
#include "SkyBox.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

int main()
{
	const int MAX_FPS = 60,
			  HEIGHT_SCENE = 200, 
			  WIDTH_SCENE = 200;
	
	static Camera* camera = new Camera(glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, "A E S T H E T I C  L A N D S C A P E", camera);
	window->setCamera(camera);

	if (!window->isOk())
		return -1;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Creating Shaders
	//Shader* terrain_shader = new Shader("vertex.glsl", "fragment.glsl", "geometry.glsl");
	Shader* terrain_shader = new Shader("vertex.glsl", "fragment.glsl", "geometry.glsl", "tessEvaluationShader.glsl", "tessControlShader.glsl");
	Shader* water_shader = new Shader("vertex_water.glsl", "fragment_water.glsl");
	Shader* skybox_shader = new Shader("SkyBoxVert.glsl", "SkyBoxFrag.glsl");

	//SKYBOX
	SkyBox* skybox = new SkyBox();
	skybox->setShader(skybox_shader);

	// HEIGHT MAP & TERRAIN
	int h = 4096, ww = 1024; // Resolution
	float HeightRange = 60;

	srand(time(NULL));
	HeightMap* hm = new HeightMap(h, HeightRange, 4, rand() % 10, 0.5);
	hm->saveMap("D:\heightMap.bmp");

	return 0;

	Object* terrain = new Object(hm->getData(), 3*h*ww);
	terrain->setIndices(hm->getIndices());
	terrain->setShader(terrain_shader);

	// WATER
	int i = 0;
	std::vector<float3>* wat_verts = new std::vector<float3>(4);
	float3 f = { HEIGHT_SCENE*1.0, 0.0, WIDTH_SCENE*1.0 };
	(*wat_verts)[i++] = f;
	f = { HEIGHT_SCENE * -1.0, 0.0, WIDTH_SCENE * 1.0 };
	(*wat_verts)[i++] = f;
	f = { HEIGHT_SCENE * 1.0, 0.0, WIDTH_SCENE * -1.0 };
	(*wat_verts)[i++] = f;
	f = { HEIGHT_SCENE * -1.0, 0.0, WIDTH_SCENE * -1.0 };
	(*wat_verts)[i++] = f;
	
	i = 0;
	std::vector<uint3>* wat_ind = new std::vector<uint3>(2);
	uint3 ind = { 0, 2, 1};
	(*wat_ind)[i++] = ind;
	ind = { 1, 2, 3 };
	(*wat_ind)[i++] = ind;

	Object* water = new Object(wat_verts);
	water->setShader(water_shader);
	water->setIndices(wat_ind);
	
	int waitFor = (int)(1 / ((float)MAX_FPS)*1000);
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.5f, 0.5f, -0.5f));

	// DEPTH TEST + ALPHA BLENDING + CULLING
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glm::vec3 lightPosition = glm::vec3(5.0f, 15.0f, 5.0f);
	model = glm::mat4(1.0f);
	float x = 0;
	
	Shader* s = NULL; // Jolly
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
		s->setData("gEyeWorldPos", camera->getPosition());
		s->setData("gDispFactor", 4.5f);
		terrain->drawObject();

		//WATER
		s = water->useShader();
		s->setData("model", model);
		s->setData("projection", camera->getProjection());
		s->setData("view", camera->getLookAt());
		water->drawObject();

		//SKYBOX
		//skybox->drawObject(camera->getLookAt(), camera->getProjection());
		
		glfwSwapBuffers(window->getWindow());
		glfwPollEvents();

		// LIMIT FPS (da rivedere)
		std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));
	}

	terrain->~Object();
	water->~Object();

	window->~Window();

	//system("PAUSE");
	return 0;
}