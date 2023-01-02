#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	// Constante initiale de la vitesse et de la vie du joueur
	const float START_SPEED = 200;
	const float START_HEALTH = 100;
	
	// Ou ce positionne le joueur 
	Vector2f m_Position;

	// Sprite du joueur
	Sprite m_Sprite;

	// La texture du joueur
	Texture m_Texture;

	// La r�solution de la fen�tre de jeu
	Vector2f m_Resolution;

	// Taille de l'arene
	IntRect m_Arena;

	// Le nombre de tile pour faire la taille de l'arene
	int m_TileSize;

	// Direction de d�placement du joueur
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	// Vie du joueur
	int m_Health;
	
	// La vie max que le joueur peut avoir
	int m_MaxHealth;

	// Quand le joueur a t'il �t� touch� la dernier fois ?
	Time m_LastHit;
	
	// Vitesse du joueur
	float m_Speed;
	
public:
	Player();

	// Fonction pour initialiser la position du joueur 
	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	// Methode appell� a chaque fin de jeu 
	void resetPlayerStats();
	
	// G�rer le fait que le joueur a �t� touch�
	bool hit(Time timeHit);

	// R�cup�rer la derni�re fois que le joueur a �t� touch�
	Time getLastHitTime();

	// Fonction pour recuperer la position du joueur
	FloatRect getPosition();
	
	// Ou ce trouve le centre du joueur
	Vector2f getCenter();
	
	// Quele angle le joueur est t'il orient�
	float getRotation();
	
	// R�cup�er le sprite du joueur pour le dessiner
	Sprite getSprite();
	
	// 4 fonction de d�placement du joueur 
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	// Fonction pour arr�ter le d�placement du joueur
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	
	// Mettre a jour la position du joueur a chaque frame
	void update(float elapsedTime, Vector2i mousePosition);
	
	// Augmenter la vitesse du joueur
	void upgradeSpeed();

	// Augmenter la vie du joueur
	void upgradeHealth();
	
	// Augmenter la capcit� du chargeur du joueur
	void increaseHealthLevel(int amount);
	
	// Recup�rer la vie actuelle du joueur
	int getHealth();
};


