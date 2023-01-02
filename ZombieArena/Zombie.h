#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
private: 
	// Vitesse de déplacement des différents énnemis
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;
	
	// La puissance de frappe des différents énnemis
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	// Faire en sorte que chaque énnemis varie légèrement sa vitesse
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;
	
	// Position de chaque énnemis
	Vector2f m_Position;

	// Sprite de chaque énnemis
	Sprite m_Sprite;
	
	// Vitesse des énnemis
	float m_Speed;

	// Vie des énnemis
	float m_Health;
	
	// Est-ce que l'ennemi est en vie ?
	bool m_Alive;

public:
	
	// Fonction qui gere le touché d'une balle sur un énnemis
	bool hit();

	// Fonction qui verifie si l'ennemi est en vie
	bool isAlive();

	// Spawn de chaque énnemis
	void spawn(float startX, float startY, int type, int seed);

	// Récupérer la position d'un énnemis
	FloatRect getPosition();

	// Récupérer le sprite d'un énnemis pour le dessiner
	Sprite getSprite();

	// Mettre a jour de l'énnemi à chaque frame
	void update(float elapsedTime, Vector2f playerLocation);

};

