#pragma once
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "heightMapThread.h"
#include "stb_image_write.h"
#include "CustomTypes.h"

typedef struct {
	_int8* coords;
} heightMap;

// sarebbe bene tirare fuori una classe qui

heightMap* NoiseMap(int dimension, float Hrange, int octaves, int primeIndex, double persistence);
heightMap* NoiseMap(int Wres, int Hres, float Hrange, float w, float h, int octaves, int primeIndex, double persistance);
void createMap(_int8* data, const char* filename, int dim);