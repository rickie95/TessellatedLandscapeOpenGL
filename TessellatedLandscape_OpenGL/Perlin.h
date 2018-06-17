#pragma once
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "CustomTypes.h"

typedef struct {
	std::vector<float3>* coords;
	std::vector<uint3>* indices;
} heightMap;

double Noise(int i, int x, int y);
double SmoothedNoise(int i, int x, int y);
double Interpolate(double a, double b, double x);
double InterpolatedNoise(int i, double x, double y);
double ValueNoise_2D(double x, double y, int numOctaves, int primeIndex, double persistence);
heightMap* createNoiseMap(int numX, int numY,float w, float h, int octaves, int primeIndex, double persistence);