#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
private: 
	// Vitesse de d�placement des diff�rents �nnemis
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;
	
	// La puissance de frappe des diff�rents �nnemis
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	// Faire en sorte que chaque �nnemis varie l�g�rement sa vitesse
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;
	
	// Position de chaque �nnemis
	Vector2f m_Position;

	// Sprite de chaque �nnemis
	Sprite m_Sprite;
	
	// Vitesse des �nnemis
	float m_Speed;

	// Vie des �nnemis
	float m_Health;
	
	// Est-ce que l'ennemi est en vie ?
	bool m_Alive;

public:
	
	// Fonction qui gere le touch� d'une balle sur un �nnemis
	bool hit();

	// Fonction qui verifie si l'ennemi est en vie
	bool isAlive();

	// Spawn de chaque �nnemis
	void spawn(float startX, float startY, int type, int seed);

	// R�cup�rer la position d'un �nnemis
	FloatRect getPosition();

	// R�cup�rer le sprite d'un �nnemis pour le dessiner
	Sprite getSprite();

	// Mettre a jour de l'�nnemi � chaque frame
	void update(float elapsedTime, Vector2f playerLocation);

};

