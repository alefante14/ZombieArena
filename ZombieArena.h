#ifndef ZOMBIE_ARENA_H
#define ZOMBIE_ARENA_H

#include <SFML/Graphics.hpp>
#include "Zombie.h"


using namespace sf;

int createBackground(VertexArray& rVA, IntRect arena);

Zombie* createHorde(int numZombies, IntRect arena);

#endif // !ZOMBIE_ARENA_H
