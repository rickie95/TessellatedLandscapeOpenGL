#version 330 core

//in vec2 FragTexCoords;

out float Color0;
out float Color1;
out float Color2;
out float Color3;
out float Color4;
out float Color5;
out float Color6;
out float Color7;

uniform float numOctaves;
uniform vec3 primes;
uniform float persistence;

float Noise(int x, int y) {

	int n = x + y * 57;
	n = (n << 13) ^ n;
	int a = int(primes[0]), b = int(primes[1]), c = int(primes[2]);
	int t = (n * (n * n * a + b) + c) & 0x7fffffff;
	return float(1.0 - t/1073741824.0);
}

float SmoothedNoise(int x, int y) {
	float corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) +
		Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16,
		sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) +
			Noise(x, y + 1)) / 8,
		center = Noise(x, y) / 4;
	return corners + sides + center;
}

float Interpolate(float a, float b, float x) {  // cosine interpolation
	float ft = x * 3.1415927,
		f = (1 - cos(ft)) * 0.5;
	return  a * (1 - f) + b * f;
}

float InterpolatedNoise(float x, float y) {
	highp int integer_X = int(x);
	float fractional_X = x - integer_X;
	highp int integer_Y = int(y);
	float fractional_Y = y - integer_Y;

	float v1 = SmoothedNoise(integer_X, integer_Y),
		v2 = SmoothedNoise(integer_X + 1, integer_Y),
		v3 = SmoothedNoise(integer_X, integer_Y + 1),
		v4 = SmoothedNoise(integer_X + 1, integer_Y + 1),
		i1 = Interpolate(v1, v2, fractional_X),
		i2 = Interpolate(v3, v4, fractional_X);
	return Interpolate(i1, i2, fractional_Y);
}

float perlin(float x, float y){
	float total = 0,
		frequency = pow(2, numOctaves),
		amplitude = 1;
	for (int i = 0; i < numOctaves; ++i) {
		frequency /= 2;
		amplitude *= persistence;

		total += InterpolatedNoise(x / frequency, y / frequency) * amplitude;
	}
	return total / frequency;
}

void main()
{
	
   //Color0 = perlin(FragTexCoords[0], FragTexCoords[1]);
   //Color1 = perlin(FragTexCoords[0] + 1, FragTexCoords[1]);
   //Color2 = perlin(FragTexCoords[0] + 2, FragTexCoords[1]);
   //Color3 = perlin(FragTexCoords[0] + 3, FragTexCoords[1]);
   //Color4 = perlin(FragTexCoords[0] + 4, FragTexCoords[1]);
   //Color5 = perlin(FragTexCoords[0] + 5, FragTexCoords[1]);
   //Color6 = perlin(FragTexCoords[0] + 6, FragTexCoords[1]);
   //Color7 = perlin(FragTexCoords[0] + 7, FragTexCoords[1]);
   
   Color0 = 0.0;
   Color1 = 1.0;
   Color2 = 2.0;
   Color3 = 3.0;
   Color4 = 4.0;
   Color5 = 5.0;
   Color6 = 6.0;
   Color7 = 7.0;
}