#include "ZombieArena.h"
#include <SFML/Graphics.hpp>

using namespace sf;


int createBackground(VertexArray& rVA, IntRect arena) {
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	//world size in tiles
	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	//what type of primitive we are using?
	rVA.setPrimitiveType(Quads);

	//set the size of the vertex array
	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);
	

	//start at the beginning of vertex array
	int currentVertex = 0;

	for (int w = 0; w < worldWidth; ++w) {
		for (int h = 0; h < worldHeight; ++h) {
			rVA[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);

			rVA[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);

			rVA[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			rVA[currentVertex + 3].position = Vector2f(w * TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			//edge of world, we use Wall
			if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1){
				// Use the wall texture
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE +TILE_TYPES * TILE_SIZE);


			}
			else{
				// Use a random floor texture
				srand((int)time(0) + h * w - h);
				int mOrG = (rand() % TILE_TYPES);  //mOrG = mud or grass
				int verticalOffset = mOrG * TILE_SIZE;
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);

			}
			//Position ready for the next four vertices
			currentVertex += VERTS_IN_QUAD;

		}
	}
	return TILE_SIZE;
}