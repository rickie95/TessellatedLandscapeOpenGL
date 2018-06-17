#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float HeightRange;
uniform int octaves;
uniform float persistance;

// AL GEOMETRY SHADER
out VS_OUT{
	vec3 FragPos;
} vs_out;

int primes[10][3] = {
	{ 995615039, 600173719, 701464987 },
	{ 831731269, 162318869, 136250887 },
	{ 174329291, 946737083, 245679977 },
	{ 362489573, 795918041, 350777237 },
	{ 457025711, 880830799, 909678923 },
	{ 787070341, 177340217, 593320781 },
	{ 405493717, 291031019, 391950901 },
	{ 458904767, 676625681, 424452397 },
	{ 531736441, 939683957, 810651871 },
	{ 997169939, 842027887, 423882827 }

};

float Noise(int i, int x, int y){
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int a = primes[i][0],
		b = primes[i][1],
		c = primes[i][2],
		t = (n * ( n * n * a + b) + c) & 0x7fffffff;
		return 1.0 - t / 1073741824.0;
}

float smoothNoise(int i, int x, int y){
	float corns = (Noise(i, x - 1, y - 1) + Noise(i, x + 1, y - 1) +
		Noise(i, x - 1, y + 1) + Noise(i, x + 1, y + 1)) / 16;

	float sides = (Noise(i, x - 1, y) + Noise(i, x + 1, y) + Noise(i, x, y - 1) +
			Noise(i, x, y + 1)) / 8,
		center = Noise(i, x, y) / 4;

	return corns + sides + center;
}

float Interp(float a, float b, float x){
	float ft = x * 3.1415927,
		f = (1 - cos(ft)) * 0.5;

	return a * ( 1 - f ) + b * f;
}

float Interpolate( int i, float x, float y){
	int int_x, int_y;
	float fract_x = x - int_x;
	float fract_y = y - int_y;
	
	float v1 = smoothNoise(i, int_x, int_y),
		v2 = smoothNoise(i, int_x + 1, int_y),
		v3 = smoothNoise(i, int_x, int_y +1 ),
		v4 = smoothNoise(i, int_x + 1, int_y),

		i1 = Interp(v1, v2, fract_x),
		i2 = Interp(v3, v4, fract_x);

	return Interp(i1, i2, fract_y);
}

float ValueNoise( float x, float y, int octaves, float persistance){
	float noise = 0.0f;
	float frequency = pow(2, octaves);
	float amplitude = 1;
	int index = 0;
	for ( int i = 0; i < octaves; i++){
		frequency /= 2;
		amplitude *= persistance;
		noise += Interpolate( (index + i) % 10, x/frequency , y/frequency) * amplitude;
	}


	return noise/frequency;
}

void main()
{
    // note that we read the multiplication from right to left
	 float Noise = ValueNoise(aPos.x, aPos.z, octaves, persistance);
     gl_Position = projection * view * model * vec4(aPos.x, aPos.y +Noise,aPos.z, 1.0);
     vs_out.FragPos = vec3(model*vec4(aPos.x,aPos.y+Noise,aPos.z,1.0));
}