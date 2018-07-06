#include "HeightMapTile.h"
#include "Object.h"
#include <chrono>

float* HeightMapTile::mapCoords = NULL;

void HeightMapTile::createMapCoord(float* input, int resolution) {
	/* Create a HeightMap in normalized coords [0,1] */
	float WidthDensity = resolution - 1;
	float HeightDensity = resolution - 1;
	int i = 0;
	for (int z = 0; z < resolution; z++) {
		for (int x = 0; x < resolution * 2; x += 2) {
			input[z*resolution * 2 + x] = (float)(i++ / (WidthDensity/2) - 1);
			input[z*resolution * 2 + x + 1] = (float)(z / (WidthDensity / 2) - 1);
			if (z == resolution - 1 )
				std::cout << input[z*resolution * 2 + x] << " " << input[z*resolution * 2 + x + 1] << std::endl;
		}
		i = 0;
	}
}

HeightMapTile::HeightMapTile(int resolution, glm::vec2 offset,int primeIndex)
{
	genShader = new Shader("TerrainGeneratorVertex.vert", "TerrainGeneratorFragment.frag");

	/* La mappa delle coordinate di base è sempre la solita e i punti sono 
	contenuti tutti nell'insieme [0,1]. In base alla risoluzione saranno più o meno densi.
	
	Bug: i punti in realtà vanno in [0, 1), lasciando una bellissima colonna e 
	riga nera nelle zone di congiunzione fra tile.

	TODO: RIVEDERE GLI INDICI NEI CICLI DI createMapCoord
	*/

	// Se la mappa non è presente allora la vado a calcolare, solo per la prima tile creata
	if (this->mapCoords == NULL) {
		this->mapCoords = (float*)malloc(sizeof(float) * 2 * resolution*resolution);
		auto before = std::chrono::high_resolution_clock::now();
		createMapCoord(this->mapCoords, resolution);
		auto after = std::chrono::high_resolution_clock::now() - before;
		auto mseconds_coords = std::chrono::duration_cast<std::chrono::milliseconds>(after).count();
		std::cout << "Texture calculated in: " << mseconds_coords << "ms" << std::endl;
	}

	Object* terrain = new Object(mapCoords, 2 * resolution * resolution, 2);
	terrain->setShader(genShader);
	terrain->setDrawMode(GL_POINTS);
	Shader* ss = terrain->useShader();

	/*	Allo shader passo la risoluzione, l'indice per la tabella dei gradienti ma
		ma soprattutto l'offset, in base al quale calcolo il valore del rumore per 
		una determinata tile.

		Il punto è: parto con [0, 1], nel fragment vengono rimappati in 
		[0, resolution] per la tile che giace nell'origine. Tutto ok.

		Per creare le altre tile mi basta aggiungere uno scostamento
		rispetto al range [0, resolution] e passare a 
		[0 + offset * resolution, (offset + 1) * resolution].

		Mi interessa solo la texture, non ha bisogno di associarla alle sue coordinate (x,y)
	*/

	ss->setData("PrimeIndex", primeIndex);
	ss->setData("resolution", resolution);
	ss->setData("offsetX", (int)offset.x);
	ss->setData("offsetY", -(int)offset.y);

	std::cout<<std::endl << (int)offset.y << std::endl;

	// CREATE RBO
	GLuint fbo = 0;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, resolution-1, resolution-1, 0, GL_RED, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FRAMEBUFFER::ERROR" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, resolution, resolution);

	auto before = std::chrono::high_resolution_clock::now();
	terrain->drawObject();
	auto after = std::chrono::high_resolution_clock::now() - before;
	auto mseconds_drawcall = std::chrono::duration_cast<std::chrono::milliseconds>(after).count();

	std::cout << "Texture calculated in: " << mseconds_drawcall << "ms" << std::endl;
	
	//free(inputPoints); // SE LA CANCELLI CHE CAZZO LA METTI STATICA A FARE COGLIONE

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);
}


HeightMapTile::~HeightMapTile()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 0, 0, 0, GL_RED, GL_FLOAT, 0);

}

GLuint HeightMapTile::getTextId()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	return textureID;
}

void HeightMapTile::enableText()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}
