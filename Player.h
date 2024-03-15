#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
	const float START_SPEED{ 200 };
	const float START_HEALTH{ 100 };

	Vector2f m_Position;

	Sprite m_Sprite;

	Texture m_Texture;
	Vector2f m_Resolution;
	//size of arena
	IntRect m_Arena;

	//How big is each tile of the arena
	int m_TileSize;

	//PLayer direction
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	int m_Health;
	int m_MaxHealth;
	//When was the player last hit
	Time m_LastHit;

	//speed in pixel/seconds
	float m_Speed;


public:
	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	void resetPlayerStats();

	bool hit(Time timeHit);

	Time getLastHitTime();

	FloatRect getPosition();

	Vector2f getCenter();
	//what angle is the player facing (3 o clock is 0 deg)
	float getRotation();

	Sprite getSprite();

	//movements
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	//call this every frame
	void update(float elapsedTime, Vector2i mousePosition);

	//boosts
	void upgradeSpeed();
	void upgradeHealth();
	void increaseHealthLevel(int amount);

	int getHealth();



};

#endif // !PLAYER_H

