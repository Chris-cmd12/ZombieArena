#include "Zombie.h"
#include "ZombieArena.h"

Zombie* createHorde(int numZombies, IntRect arena)
{
	// Tableau de pointeurs qui stock les énemis
	Zombie* zombies = new Zombie[numZombies];
	
	int maxY = arena.height -20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	// Spawn des énemis
	for (int i = 0; i < numZombies; i++)
	{
		// De quel coté ils spawn
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;
		
		switch (side)
		{
		case 0:
			// Left
			x = minX;
			y = (rand() % maxY) + minY;
			break;
			
		case 1:
			// Right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;
		case 2:
			
			// Top
			x = (rand() % maxX) + minX;
			y = minY;
			break;
			
		case 3:
			
			// Bottom
			x = (rand() % maxX) + minX;
			y = maxY;
			break;
		}
		
		// Quel type d'énemi spawn
		srand((int)time(0) * i * 2);
		int type = (rand() % 3);

		// Faire spawn l'énemie dans in tableau
		zombies[i].spawn(x, y, type, i);
	}
	return zombies;
}