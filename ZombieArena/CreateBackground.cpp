#include "SFML/Graphics.hpp"
#include "ZombieArena.h"

int createBackground(VertexArray& rVA, IntRect arena) {
	
	// Tout ce que nous faisons à rVA, nous le faisons en réalité
	// à l'arrière-plan (dans la fonction principale)
	// Quelle est la taille de chaque tuile/texture ?

	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	// Taille de l'arène en nombre de tuiles
	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	// Quel type de primitif utilisons-nous ?
	rVA.setPrimitiveType(Quads);
	
	// Taille du tableau de vertex
	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	// Commencez au début du tableau de vertex
	int currentVertex = 0;
	
	for (int w = 0; w < worldWidth; w++) {
		for (int h = 0; h < worldHeight; h++) {
			// Positionner chaque vertex dans le quad actuel
			rVA[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);

			rVA[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f(w * TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			// Définir la position de la texture pour chaque vertex
			// Même les buissons, l'herbes et les murs 
			if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1) {
				// This is a wall
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else {
				// Utiliser un nombre aléatoire pour choisir le type de tuile
				srand((int)time(0) + h * w - h);
				int mOrG = (rand() % TILE_TYPES);
				int verticalOffset = mOrG * TILE_SIZE;

				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);
				
			}

			// Initialiser les position pour les 4 vertex du quad suivant 
			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}
	
	

	return TILE_SIZE;
}
