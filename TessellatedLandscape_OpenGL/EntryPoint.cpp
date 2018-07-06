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
#include <chrono>
#include <stdlib.h>
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "HeightMap.h"
#include "Object.h"
#include "CustomTypes.h"
#include "SkyBox.h"
#include "HeightMapTile.h"

const int SCR_WIDTH = 800,
			SCR_HEIGHT = 600,
			MAX_FPS = 60,
			HEIGHT_SCENE = 200,
			WIDTH_SCENE = 200;

unsigned int loadTexture(char const* path);
/*
void createIndices(std::vector<uint3>* indices, int resolution) {
	unsigned int i = 0;
	for (int z = 0; z < resolution; z++) {
		for (int x = 0; x < resolution; x++) {
			// Nel frattempo creo gli indici
			if (x + 1 < resolution && z + 1 < resolution) {
				uint3 tri = { i, i + resolution, i + resolution + 1 };
				uint3 tri2 = { i, i + resolution + 1, i + 1 };
				indices->push_back(tri);
				indices->push_back(tri2);
			}
			i++;
		}
	}
}*/

int main()
{
	srand(time(NULL));
	
	static Camera* camera = new Camera(glm::vec3(0.0f, 3.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, "A E S T H E T I C  L A N D S C A P E", camera);
	window->setCamera(camera);

	if (!window->isOk()) {
		std::cout << "NO" << std::endl;
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Creating Shaders
	Shader* terrain_shader = new Shader("ColorTerrainVert.vert", "ColorTerrainFrag.frag");

	// HEIGHT MAP
	int resolution = 512; // Resolution
	glm::vec2 offset = { 0.0, 0.0 };
	int seed = rand() % 10;

	// Creo le tile. Indico la risoluzione e l'offset rispetto a (0,0), è importante che 
	// il seed sia lo stesso per ogni tile in modo da avere continuità.
	HeightMapTile* hmt = new HeightMapTile(resolution, glm::vec2(0, 0), seed );
	HeightMapTile* hmt_NORTH = new HeightMapTile(resolution, glm::vec2(0, 1), seed);
	HeightMapTile* hmt_N_EAST = new HeightMapTile(resolution, glm::vec2(1, 1), seed);
	HeightMapTile* hmt_EAST = new HeightMapTile(resolution, glm::vec2(1, 0), seed);
	HeightMapTile* hmt_S_EAST = new HeightMapTile(resolution, glm::vec2(1, -1), seed);
	HeightMapTile* hmt_SOUTH = new HeightMapTile(resolution, glm::vec2(0, -1), seed);
	HeightMapTile* hmt_S_WEST = new HeightMapTile(resolution, glm::vec2(-1, -1), seed);
	HeightMapTile* hmt_WEST = new HeightMapTile(resolution, glm::vec2(-1, 0), seed);
	HeightMapTile* hmt_N_WEST = new HeightMapTile(resolution, glm::vec2(-1, 1), seed);
	
	

	/*
	Shader* genShader = new Shader("TerrainGeneratorVertex.vert", "TerrainGeneratorFragment.frag");

	float* inputPoints = (float*)malloc(sizeof(float) * 2 * resolution*resolution);

	auto before = std::chrono::high_resolution_clock::now();
	createMapCoord(inputPoints, resolution);
	auto after = std::chrono::high_resolution_clock::now() - before;
	auto mseconds_coords = std::chrono::duration_cast<std::chrono::milliseconds>(after).count();

	Object* terrain = new Object(inputPoints, 2 * resolution * resolution, 2);
	terrain->setShader(genShader);
	terrain->setDrawMode(GL_POINTS);
	Shader* ss = terrain->useShader();
	ss->setData("PrimeIndex", (int)rand() % 10);
	ss->setData("resolution", resolution);
	ss->setData("offset", offset);
	glEnable(GL_DEPTH_TEST);
	// CREATE RBO
	GLuint fbo = 0, textureID = 0;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, resolution, resolution, 0, GL_RED, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FRAMEBUFFER::ERROR" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, resolution, resolution);

	before = std::chrono::high_resolution_clock::now();
	terrain->drawObject();
	after = std::chrono::high_resolution_clock::now() - before;
	auto mseconds_drawcall = std::chrono::duration_cast<std::chrono::milliseconds>(after).count();

	std::cout << "Cords calculated in: " << mseconds_coords << "ms" << std::endl;
	std::cout << "Texture calculated in: " << mseconds_drawcall << "ms" << std::endl;

	float* aRow = new float[resolution*resolution];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, aRow);
	for (int i = 0; i < 10; i++) {
		std::cout << aRow[i] << " "<<std::endl;
	}

	free(inputPoints); // SICURO?

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);
	*/
	// END OF HEIGHTMAP CREATION
	
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	/*
	std::vector<uint3>* indices = new std::vector<uint3>();
	indices->push_back({ 0, 1, 2 });
	indices->push_back({ 1, 2, 3 });
	*/

	
	int waitFor = (int)(1 / ((float)MAX_FPS) * 1000);
	glm::vec3 lightPosition = glm::vec3(5.0f, 15.0f, 5.0f);
	Shader* s = NULL; // Jolly

	
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Il piano di soli quattro vertici che utilizzo per appiccicare la texture
	float quadVertices[] = { 
		1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		
		1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f
	};
	
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	std::cout << "Initit phase done." << std::endl;

	int displacement = 5;
	Shader* tile_shader = new Shader("ColorTerrainVert.vert", "ColorTerrainFrag.frag");
	glDisable(GL_DEPTH_TEST);
	while (!window->isClosed())
	{
		window->processInput();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(10.0));
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		tile_shader->use();
		tile_shader->setData("projection", camera->getProjection());
		tile_shader->setData("view", camera->getLookAt());
		tile_shader->setData("model", model);

		/* Nel loop non faccio altro che disegnare lo stesso piano più volte, 
		   traslando in senso orario per creare una griglia 9x9.
		   Ogni volta ricalcolo model e abilito la texture giusta.
		*/

		// ORIGINE
		hmt->enableText();
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//NORTH
		hmt_NORTH->enableText();
		model = glm::translate(model, glm::vec3(0.0, 0.0, -2.0 * displacement));
		tile_shader->setData("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		//NORTH EAST
		hmt_N_EAST->enableText();
		model = glm::translate(model, glm::vec3(2.0 * displacement , 0.0, 0.0));
		tile_shader->setData("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		//EAST
		hmt_EAST->enableText();
		model = glm::translate(model, glm::vec3(0.0, 0.0, 2.0 * displacement ));
		tile_shader->setData("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//SOUTH EAST
		hmt_S_EAST->enableText();
		model = glm::translate(model, glm::vec3(0.0, 0.0, 2.0 * displacement));
		tile_shader->setData("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//SOUTH
		hmt_SOUTH->enableText();
		model = glm::translate(model, glm::vec3(-2.0 * displacement , 0.0, 0.0));
		tile_shader->setData("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//SOUTH WEST
		hmt_S_WEST->enableText();
		model = glm::translate(model, glm::vec3(-2.0 * displacement, 0.0, 0.0));
		tile_shader->setData("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//WEST
		hmt_WEST->enableText();
		model = glm::translate(model, glm::vec3(0.0, 0.0, -2.0 * displacement));
		tile_shader->setData("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// NORTH WEST
		hmt_N_WEST->enableText();
		model = glm::translate(model, glm::vec3(0.0, 0.0, -2.0 * displacement));
		tile_shader->setData("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glfwSwapBuffers(window->getWindow());
		glfwPollEvents();
		// LIMIT FPS (da rivedere)
		std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));
	}

	return 0;


	/*
	Object* terrain = new Object(hm->getData(20,20), 3*resolution*resolution);
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
	*/
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}