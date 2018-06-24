
#include "HeightMap.h"


heightMap* NoiseMap(int Wres,int Hres, float Hrange, float w, float h,int octaves, int primeIndex, double persistance){
	
	heightMap* hm = new heightMap;
	std::vector<uint3>* ind = new std::vector<uint3>;

	float* vv = (float*) malloc(sizeof(float)*Wres*Hres*3);
	unsigned int i = 0;
	std::vector<NoiseThread*> tt(Hres); 

	for (int z = 0; z < Hres; z++) {
		tt[z] = new NoiseThread(vv, z, Wres, Hres, Hrange, w, h, octaves, primeIndex, persistance);
		for (int x = 0; x < Wres; x++) {
			// Nel frattempo creo gli indici
			if (x + 1 < Wres && z + 1 < Hres) {
				uint3 tri = { i, i + Wres, i + Wres + 1 };
				uint3 tri2 = { i, i + Wres + 1, i + 1 };
				ind->push_back(tri);
				ind->push_back(tri2);
			}
			i++;
		}
	}

	for (int z = 0; z < Hres; z++) {
		tt[z]->join();
	}
	hm->coords = vv;
	hm->indices = ind;

	return hm;
}