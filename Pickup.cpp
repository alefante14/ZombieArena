#include "Pickup.h"
#include "TextureHolder.h"
#include <iostream>

Pickup::Pickup(int type) {
	m_Type = type;
	if (type == 1) {
		m_Value = HEALTH_START_VALUE;
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/health_pickup.png"));
	}
	else if(type == 2){
		m_Value = AMMO_START_VALUE;
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/ammo_pickup.png"));

	}
	else {
		exit(EXIT_FAILURE);
	}
	m_Sprite.setOrigin(25, 25);
	m_SecondsToLive = START_SECONDS_TO_LIVE;
	m_SecondsToWait = START_WAIT_TIME;
}

void Pickup::setArena(IntRect arena){
	// Copy the details of the arena to the pickup's m_Arena
	m_Arena.left = arena.left + 75;
	m_Arena.width = arena.width - 150;
	m_Arena.top = arena.top + 75;
	m_Arena.height = arena.height - 150;
	spawn();
}

void Pickup::spawn() {
	// Spawn at a random location
	srand((int)time(0) / m_Type);
	int x = (m_Arena.left + rand() % m_Arena.width);
	srand((int)time(0) * m_Type);
	int y = (m_Arena.top + rand() % m_Arena.height);
	m_SecondsSinceSpawn = 0;
	m_Spawned = true;
	m_Sprite.setPosition(x, y);
}

FloatRect Pickup::getPosition(){
	return m_Sprite.getGlobalBounds();
}
Sprite Pickup::getSprite(){
	return m_Sprite;
}
bool Pickup::isSpawned(){
	return m_Spawned;
}

int Pickup::gotIt() {
	m_Spawned = false;
	m_SecondsSinceDeSpawn = 0;
	return m_Value;
}

void Pickup::update(float elapsedTime){
	if (m_Spawned){
		m_SecondsSinceSpawn += elapsedTime;
	}
	else{
		m_SecondsSinceDeSpawn += elapsedTime;
	}
	// Do we need to hide a pickup?
	if (m_SecondsSinceSpawn > m_SecondsToLive && m_Spawned){
		// Remove the pickup and put it somewhere else
		m_Spawned = false;
		m_SecondsSinceDeSpawn = 0;
	}
	// Do we need to spawn a pickup
	if (m_SecondsSinceDeSpawn > m_SecondsToWait && !m_Spawned){
		// spawn the pickup and reset the timer
		spawn();
	}
}

void Pickup::upgrade() {
	if (m_Type == 1)
	{
		m_Value += (HEALTH_START_VALUE * .5);
	}
	else
	{
		m_Value += (AMMO_START_VALUE * .5);
	}
	// Make them more frequent and last longer
	m_SecondsToLive += (START_SECONDS_TO_LIVE / 10);
	m_SecondsToWait -= (START_WAIT_TIME / 10);
}