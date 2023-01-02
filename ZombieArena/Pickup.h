#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup
{
private:
	// Valeur initial des kit de soin et munitions
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	
	// Temps de respawn des kit de soin et munitions
	const int START_WAIT_TIME = 10;
	
	// Temps de vie des kit de soin et munitions
	const int START_SECONDS_TO_LIVE = 5;

	// Sprite des différent pickup
	Sprite m_Sprite;

	// Arene du jeu
	IntRect m_Arena;
	
	// La veleur du pick up
	int m_Value;

	// Quel type de pickup est-ce ?
	// 1 = kite de soin, 2 = munitions
	int m_Type;

	// Variable pour gérer l'apparition des pickup
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

public:
	Pickup(int type);

	// Préparer un nouveau pickup
	void setArena(IntRect arena);
	
	void spawn();

	// Verifier la position du pickup
	FloatRect getPosition();

	// Recupérer son sprite pour le dessiner 
	Sprite getSprite();

	// Mettre a jour les pickup a chaque frame
	void update(float elapsedTime);
	
	// Est ce que le pick up a spawn ?
	bool isSpawned();

	// Récuperation des pickup 
	int gotIt();

	// Amelioration des pickup
	void upgrade();
};

