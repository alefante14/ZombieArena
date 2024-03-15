#include "Bullet.h"

using namespace sf;

Bullet::Bullet() {
	m_BulletShape.setSize(Vector2f(3, 3));
}

void Bullet::shoot(float startX, float startY, float xTarget, float yTarget) {
	m_Position.x = startX;
	m_Position.y = startY;
	m_InFlight = true;

	//calculate gradient of flight path
	float gradient = (startX - xTarget )/ (startY - yTarget);
	if (gradient < 0) {
		gradient *= -1;
	}

	//calculate the ratio between x and y
	float ratioXY = m_BulletSpeed / (1 + gradient);

	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	if (xTarget < startX) {
		m_BulletDistanceX *= -1;
	}
	if (yTarget < startY) {
		m_BulletDistanceY *= -1;
	}
	//set max range of 1000 pixels
	float range = 2000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	m_BulletShape.setPosition(m_Position);
}

void Bullet::stop() {
	m_InFlight = false;
}

bool Bullet::isInFlight() {
	return m_InFlight;
}

FloatRect Bullet::getPosition() {
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape() {
	return m_BulletShape;
}

void Bullet::update(float elapsedTime) {
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	m_BulletShape.setPosition(m_Position);
	// Has the bullet gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY){
		m_InFlight = false;
	}
}