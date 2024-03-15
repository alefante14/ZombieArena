#ifndef PICKUP_H
#define PICKUP_H

#include <SFML/Graphics.hpp>

using namespace sf;


class Pickup {
private:
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 5;
	const int START_SECONDS_TO_LIVE = 15;

	Sprite m_Sprite;
	IntRect m_Arena;

	//how much is the pickup worth?
	int m_Value;
	
	//1 = health, 2 = ammo
	int m_Type;

	// Handle spawning and disappearing
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

public:
	Pickup(int type);
	
	//Prepare a new pickup
	void setArena(IntRect arena);

	void spawn();

	//check the pos of a pickup
	FloatRect getPosition();
	Sprite getSprite();
	
	// Let the pickup update itself each frame
	void update(float elapsedTime);
	// Is this pickup currently spawned?
	bool isSpawned();
	// Get the goodness from the pickup
	int gotIt();
	// Upgrade the value of each pickup
	void upgrade();

};

#endif // !PICKUP_H
