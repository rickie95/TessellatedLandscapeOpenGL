#pragma once
#include "Shader.h"

class HeightMapTile
{
private:
	GLuint textureID;
	int resolution;
	Shader* genShader;
	static float* mapCoords; // perchè ricalcolarla ogni volta?

	void createMapCoord(float* map, int resolution);

public:
	HeightMapTile(int resolution, glm::vec2 offset, int primeIndex);
	~HeightMapTile();
	GLuint getTextId();
	void enableText();
};



