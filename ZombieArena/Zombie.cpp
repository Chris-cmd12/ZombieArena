#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void Zombie::spawn(float startX, float startY, int type, int seed)
{
	// Place the zombie at the starting location
	switch (type)
	{
	case 0:
		// Bloater
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		m_Speed = 40;
		m_Health = 5;
		break;
	case 1:
		// Chaser
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		m_Speed = 70;
		m_Health = 1;
		break;
	case 2:
		// Crawler
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = 20;
		m_Health = 3;
		break;
	}

	// Modification de la vitesse de l'ennemi
	// Chaque enemi est unique.
	srand((int)time(0) * seed);
	// Leur différence et entre 80 et 100
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;

	// Ce si et une division par 1
	modifier /= 100; // la différence de speed et de 0.7 et 1
	m_Speed *= modifier;

	// Position de départ de l'ennemi
	m_Position.x = startX;
	m_Position.y = startY;

	// Origine du sprite au centre
	m_Sprite.setOrigin(25, 25);

	// Placer son sprite à la bonne position
	m_Sprite.setPosition(m_Position);
}

bool Zombie::hit()
{
	m_Health--;
	
	if (m_Health < 0)
	{
		// mort
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}

	// Bléssé mais pas mort
	return false;
}

bool Zombie::isAlive()
{
	return m_Alive;
}

FloatRect Zombie::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Zombie::getSprite()
{
	return m_Sprite;
}

void Zombie::update(float elapsedTime, Vector2f playerLocation)
{
	// Mettre a jour la position de l'énemi
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	// Mettre a jour la position de l'énemi
	if (playerX > m_Position.x)
	{
		m_Position.x = m_Position.x + m_Speed * elapsedTime;
	}

	if (playerY > m_Position.y)
	{
		m_Position.y = m_Position.y + m_Speed * elapsedTime;
	}

	if (playerX < m_Position.x)
	{
		m_Position.x = m_Position.x - m_Speed * elapsedTime;
	}

	if (playerY < m_Position.y)
	{
		m_Position.y = m_Position.y - m_Speed * elapsedTime;
	}

	// Déplacer le sprite
	m_Sprite.setPosition(m_Position);

	// Tourner le sprite vers le joueur
	float angle = (atan2(playerY - m_Position.y, playerX - m_Position.x) * 180) / 3.141;
	
	m_Sprite.setRotation(angle);
}
