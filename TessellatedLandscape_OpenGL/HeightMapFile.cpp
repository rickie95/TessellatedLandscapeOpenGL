#pragma once
#include "HeightMapFile.h"



void createMap(_int8* data, const char * filename, int dim)
{
	//int stbi_write_bmp(char const *filename, int w, int h, int comp, const void *data);
	if (stbi_write_bmp(filename, dim, dim, 1, data)) {
		std::cout << "Height map completed" << std::endl;
	}
	else {
		std::cout << "Height map completed" << std::endl;
	}
}

heightMap * NoiseMap(int res, float Hrange, int octaves, int primeIndex, double persistance) {

	heightMap* hm = new heightMap;
	std::vector<heightMapThread*> tt(res);

	// heightmap matrix
	float* vv = (float*)malloc(sizeof(float)*res*res);

	

	for (int z = 0; z < res; z++) {
		tt[z] = new heightMapThread(vv, z, res, Hrange, octaves, primeIndex, persistance);
	}

	threadValues maxMin;
	float max=0, min=0;
	for (int z = 0; z < res; z++) {
		// gather max and min values from every thread
		maxMin = tt[z]->join();
		if (maxMin.max > max)
			max = maxMin.max;
		if (maxMin.min < min)
			min = maxMin.min;
	}


	_int8* data = (_int8*)malloc(sizeof(_int8)*res*res);

	//map data in 0-255 range
	//  (- min , + max) -> ( 0, max + min) -> ( 0 , 255)
	float valuex;
	for (int j = 0; j < res*res; j++) {
		valuex = (vv[j] + min )* 255/max ;
		data[j] = (_int8)valuex;
	}


	hm->coords = data;


	return hm;
}

heightMap * NoiseMap(int dimension, float Hrange, float density, int octaves, int primeIndex, double persistence)
{
	return NoiseMap(dimension, dimension, Hrange, density, density, octaves, primeIndex, persistence);
}
