#pragma once
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "heightMapThread.h"
#include "stb_image_write.h"
#include "CustomTypes.h"

class HeightMap {

private:
	float* coords = NULL;
	std::vector<uint3>* indices;
	float max, min;
	int resolution;

public:
	HeightMap(int dimension, float Hrange, int octaves, int primeIndex, double persistence);
	~HeightMap();
	float* getData();
	std::vector<uint3>* getIndices();
	int getResolution();
	void saveMap(const char* filename);

};