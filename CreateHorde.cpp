#include "ZombieArena.h"
#include "Zombie.h"
#include <iostream>

Zombie* createHorde(int numZombies, IntRect arena) {
	Zombie* zombies = new Zombie[numZombies];

	int maxY = arena.height - 100;
	int minY = arena.top + 50;
	int maxX = arena.width - 100;
	int minX = arena.left + 50;

	for (int i = 0; i < numZombies; ++i) {
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;

		switch (side)
		{
		case 0:
			//left
			x = minX;
			y = (rand() % maxY) + minY;
			break;
		case 1:
			//right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;
		case 2:
			//top
			x = (rand() % maxX) + minX;
			y = minY;
			break;
		case 3:
			//bottom
			x = (rand() % maxX) + minX;
			y = maxY;
			break;
		}
		//zombie type
		srand((int)time(0) * i * 2);
		zombieType type = zombieType(rand() % 3);


		zombies[i].spawn(x, y,type, i);
		std::cout << "ID = " << i << "  x = " << x << "  y = " << y << "is alive = " << zombies[i].isAlive() << "\n";

	}
		return zombies;
	
	
}