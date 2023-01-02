#include "Bullet.h"

Bullet::Bullet()
{
	// Taille de la balle
	m_BulletShape.setSize(sf::Vector2f(2, 2));
}

void Bullet::shoot(float startX, float startY, float xTarget, float yTarget){

	// suivre le déplacement de la balle
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	// Calcule de la trajectoire de la balle
	float gradient = (startX - xTarget) / (startY - yTarget);

	// Tout les trajectoires inferrieur a 0 sont négatives
	if (gradient < 0)
	{
		gradient *= -1;
	}

	// Calculer le ration entre x et y
	float ratioXY = m_BulletSpeed / (1 + gradient);

	// initialiser le vitesses horizontale et verticale de la balle
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	// Envoyer la balle dans la bonne direction
	if (xTarget < startX)
	{
		m_BulletDistanceX *= -1;
	}
	
	if (yTarget < startY)
	{
		m_BulletDistanceY *= -1;
	}

	// initialiser la taille maximal
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;
	
	// Positioner la balle qui est pret à être déssiner 
	m_BulletShape.setPosition(m_Position);
}

void Bullet::stop()
{
	m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}

void Bullet::update(float elapsedTime)
{
	// Mettre a jour la position de la balle
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	// Faire bouger la balle
	m_BulletShape.setPosition(m_Position);

	// Si la balle sort de l'écran, la stopper
	if (m_Position.x > m_MaxX || m_Position.x < m_MinX ||
		m_Position.y > m_MaxY || m_Position.y < m_MinY)
	{
		m_InFlight = false;
	}
}