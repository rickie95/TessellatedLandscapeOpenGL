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

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

typedef struct{
	float x, y, z;
}float3;

typedef struct {
	unsigned int x, y, z;
}uint3;

int main()
{
	int h = 128, ww = 128;
	double* heighMap = createNoiseMap(h, ww, 7, rand()%10, 0.5);
	
	for (int y = 0; y < h; y += 1) {
		for (int x = 0; x < ww; x += 1) {
			printf(" %f ", heighMap[x*ww + y]);
		}
		printf("\n");
	}

	
	const int MAX_FPS = 60;

	static Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Window* window = new Window(800, 600, "A E S T H E T I C  L A N D S C A P E", camera);
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
	Shader* LightShader = new Shader("lightVertex.glsl", "lightFragment.glsl");
	

	// VERTEX DATA
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.2f, -0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
		0.2f, -0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
		0.2f,  0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
		0.2f,  0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
		-0.2f,  0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
		-0.2f, -0.2f, -0.2f,  0.0f,  0.0f, -1.0f,

		-0.2f, -0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
		0.2f, -0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
		0.2f,  0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
		0.2f,  0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
		-0.2f,  0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
		-0.2f, -0.2f,  0.2f,  0.0f,  0.0f,  1.0f,

		-0.2f,  0.2f,  0.2f, -1.0f,  0.0f,  0.0f,
		-0.2f,  0.2f, -0.2f, -1.0f,  0.0f,  0.0f,
		-0.2f, -0.2f, -0.2f, -1.0f,  0.0f,  0.0f,
		-0.2f, -0.2f, -0.2f, -1.0f,  0.0f,  0.0f,
		-0.2f, -0.2f,  0.2f, -1.0f,  0.0f,  0.0f,
		-0.2f,  0.2f,  0.2f, -1.0f,  0.0f,  0.0f,

		0.2f,  0.2f,  0.2f,  1.0f,  0.0f,  0.0f,
		0.2f,  0.2f, -0.2f,  1.0f,  0.0f,  0.0f,
		0.2f, -0.2f, -0.2f,  1.0f,  0.0f,  0.0f,
		0.2f, -0.2f, -0.2f,  1.0f,  0.0f,  0.0f,
		0.2f, -0.2f,  0.2f,  1.0f,  0.0f,  0.0f,
		0.2f,  0.2f,  0.2f,  1.0f,  0.0f,  0.0f,

		-0.2f, -0.2f, -0.2f,  0.0f, -1.0f,  0.0f,
		0.2f, -0.2f, -0.2f,  0.0f, -1.0f,  0.0f,
		0.2f, -0.2f,  0.2f,  0.0f, -1.0f,  0.0f,
		0.2f, -0.2f,  0.2f,  0.0f, -1.0f,  0.0f,
		-0.2f, -0.2f,  0.2f,  0.0f, -1.0f,  0.0f,
		-0.2f, -0.2f, -0.2f,  0.0f, -1.0f,  0.0f,

		-0.2f,  0.2f, -0.2f,  0.0f,  1.0f,  0.0f,
		0.2f,  0.2f, -0.2f,  0.0f,  1.0f,  0.0f,
		0.2f,  0.2f,  0.2f,  0.0f,  1.0f,  0.0f,
		0.2f,  0.2f,  0.2f,  0.0f,  1.0f,  0.0f,
		-0.2f,  0.2f,  0.2f,  0.0f,  1.0f,  0.0f,
		-0.2f,  0.2f, -0.2f,  0.0f,  1.0f,  0.0f
		
		
	};

	float lightCubeVertices[] = {
		0.1f, 0.1f, 0.1f,	1.0f, 1.0f, 1.0f,//top right
		0.1f, -0.1, 0.1f,	1.0f, 1.0f, 1.0f,// bottom right
		-0.1f, -0.1f, 0.1f,	1.0f, 1.0f, 1.0f, // bottom left
		-0.1f,  0.1f, 0.1f,  1.0f, 1.0f, 1.0f,// top left

		//REAR
		0.1f, 0.1f, 0.0f,	1.0f, 1.0f, 1.0f,//top right
		0.1f, -0.1f, 0.0f,  1.0f, 1.0f, 1.0f,// bottom right
		-0.1f, -0.1f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom left
		-0.1f,  0.1f, 0.0f, 1.0f, 1.0f, 1.0f// top left
	};

	unsigned int lightCubeIndices[] = {
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
		3, 6, 7,
	};	


	// FIRST CUBE AND FLOOR
	
	unsigned int VBO, VAO;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// HEIGHT MAP
	int Wres = 128, Hres = 128;
	const int numberOfVertex = Wres * Hres;
	std::vector<float3> verts(numberOfVertex);
	std::vector<uint3> tris;

	const float w = 10.f, l = 10.0f;

	int i = 0;
	float zOff = 0;
	for (int z = 0; z < Hres; z++) {
		float xOff = 0;
		for (int x = 0; x < Wres; x++) {
			float3 v;
			v.x = x / (float)Wres;
			v.y = 0;
			v.z = z / (float)Hres;

			//scale
			v.x *= w;
			v.z *= l;

			//move
			v.x -= w / 2;
			v.z -= l / 2;

			v.y = heighMap[x*Hres + z]*10;

			verts[i] = v;

			if ((i + 1 % Wres) != 0 && z + 1 < Hres) {
				uint3 tri = { i, i + Wres, i + Wres + 1 };
				uint3 tri2 = { i, i + Wres + 1, i + 1 };

				tris.push_back(tri);
				tris.push_back(tri2);
			}

			i++;
			xOff += 0.1;
		}
		zOff += 0.1;
	}
	int TerrainCount = tris.size();

	// LIGHT CUBE
	unsigned int VBO_LC, VAO_LC, EBO_LC;
	glGenVertexArrays(1, &VAO_LC);
	glGenBuffers(1, &VBO_LC);
	glGenBuffers(1, &EBO_LC);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LC);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float3)*verts.size(), verts.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO_LC);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// now EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_LC);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint3)*tris.size(), tris.data(), GL_STATIC_DRAW);
	std::cout << sizeof(uint3)/3 << std::endl;
	std::cout << sizeof(GL_UNSIGNED_INT) << std::endl;
	int waitFor = 1 / ((float)MAX_FPS)*1000;

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.5f, 0.5f, -0.5f));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
	model = glm::mat4(1.0f);

	while (!window->isClosed())
	{
		window->processInput();

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// be sure to activate the shader before any calls to glUniform
		LightShader->use();
		//model = glm::mat4(1.0f);
		LightShader->setMat4("projection", camera->getProjection());
		LightShader->setMat4("model", model);
		LightShader->setMat4("view", camera->getLookAt());
		LightShader->setVec3("lightPos", lightPosition);
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 36);
		glBindVertexArray(0);
		

		// LightCube
		shader->use();
		//model = glm::translate(model, lightPosition);
		//model = glm::mat4(1.0f);
		shader->setMat4("projection", camera->getProjection());
		shader->setMat4("model", model);
		shader->setMat4("view", camera->getLookAt());
		glBindVertexArray(VAO_LC);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_LC);
		glDrawElements(GL_LINES, tris.size()*3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		glfwSwapBuffers(window->getWindow());
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));
		
		//window->Close();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO_LC);
	glDeleteBuffers(1, &VBO_LC);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	window->~Window();

	//system("PAUSE");
	return 0;
}

