#pragma once
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "NoiseThread.h"
#include "CustomTypes.h"

typedef struct {
	//std::vector<float3>* coords;
	float* coords;
	std::vector<uint3>* indices;
} heightMap;

heightMap* createNoiseMap(int numX, int numY,float Hrange,float w, float h, int octaves, int primeIndex, double persistence);