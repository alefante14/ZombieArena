#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Graphics.hpp>
using namespace sf;


const enum zombieType { BLOATER, CHASER, CRAWLER }; //modify the type with enum later


class Zombie {
private:
	//how fast is each zombie 
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//how tough is each zombie
	const float BLOATHER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//make each zombie vary its speed slightly
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	Vector2f m_Position;
	Sprite m_Sprite;
	float m_Speed;
	float m_Health;
	bool m_Alive = true;

public:
	bool hit();
	bool isAlive();

	void spawn(float startX, float startY, zombieType type, int seed);
	FloatRect getPosition();
	Sprite getSprite();
	void update(float elapsedTime, Vector2f playerLocation);
};

#endif // !ZOMBIE_H
