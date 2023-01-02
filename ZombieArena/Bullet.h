#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
private:
	// ou ce position les balles
	Vector2f m_Position;
	
	// La forme de la balle et son apparence
	RectangleShape m_BulletShape;
	
	// Est-ce que la balle est en mouvement
	bool m_InFlight = false;
	
	// La vitesse de la balle
	float m_BulletSpeed = 1000;
	
	// Quelle fraction de 1 pixel la balle parcourt-elle, horizontalement et verticalement, à chaque image ?
	float m_BulletDistanceX;
	float m_BulletDistanceY;
	
	// bordure pour que la balle disparaisse
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;
	
public:
	Bullet();

	// Stop la balle
	void stop();

	// Retoune la valeur de m_InFlight
	bool isInFlight();

	// Tirer une balle
	void shoot(float startX, float startY, float xTarget, float yTarget);

	// Récupération de la position de la balle
	FloatRect getPosition();

	// Retoure la forme de la balle pour la dessiner
	RectangleShape getShape();

	// Mettre à jour la balle à chaque frame
	void update(float elapsedTime);
};


