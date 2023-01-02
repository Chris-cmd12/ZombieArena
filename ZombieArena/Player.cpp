#include "Player.h"
#include <iostream>
#include <sstream>
#include "TextureHolder.h"

using namespace std;

// Player constructor
Player::Player() {
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	// Associer le sprite au texture
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player.png"));
	// Set l'origin au centre du sprite
	m_Sprite.setOrigin(25, 25);
}

// Faire spawn le joueur au centre de l'arene
void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) {
	// Place le joeur milieu de l'arene
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	// Copie les detail de l'arene dans m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	// Garder la taille de la tile
	m_TileSize = tileSize;

	// Garder la resolution
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

// Réinitialiser le joueur à la fin de la partie
void Player::resetPlayerStats() {
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

// Dernier fois que le joueur a été touché
Time Player::getLastHitTime() {
	return m_LastHit;
}

// Gestion de joueur ce faisant toucher par un énemis
bool Player::hit(Time timeHit) {
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200) {
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else {
		return false;
	}
}

// Ou ce trouve le joueur
FloatRect Player::getPosition() {
	return m_Sprite.getGlobalBounds();
}

// Ou ce trouve le centre du joueur
Vector2f Player::getCenter() {
	return m_Position;
}

// Quelle angle le joueur est t'il orienté
float Player::getRotation() {
	return m_Sprite.getRotation();
}

// Envoie une copie de la texture du joueur
Sprite Player::getSprite() {
	return m_Sprite;
}

// Recupérer la vie du joueur
int Player::getHealth() {
	return m_Health;
}

// Dépalcement du joueur
void Player::moveLeft() {
	m_LeftPressed = true;
}

void Player::moveRight() {
	m_RightPressed = true;
}

void Player::moveUp() {
	m_UpPressed = true;
}

void Player::moveDown() {
	m_DownPressed = true;
}

// Arréter son déplacement
void Player::stopLeft() {
	m_LeftPressed = false;
}

void Player::stopRight() {
	m_RightPressed = false;
}

void Player::stopUp() {
	m_UpPressed = false;
}

void Player::stopDown() {
	m_DownPressed = false;
}

// Mettre a jour le joueur a chaque frame
void Player::update(float elapsedTime, Vector2i mousePosition) {
	if (m_UpPressed) {
		m_Position.y -= m_Speed * elapsedTime;
	}

	if (m_DownPressed) {
		m_Position.y += m_Speed * elapsedTime;
	}

	if (m_RightPressed) {
		m_Position.x += m_Speed * elapsedTime;
	}

	if (m_LeftPressed) {
		m_Position.x -= m_Speed * elapsedTime;
	}
	
	m_Sprite.setPosition(m_Position);
	
	// Gérer la collision avec l'arene
	if (m_Position.x > m_Arena.width - m_TileSize) {
		m_Position.x = m_Arena.width - m_TileSize;
	}
	
	if (m_Position.x < m_Arena.left + m_TileSize) {
		m_Position.x = m_Arena.left + m_TileSize;
	}

	if (m_Position.y > m_Arena.height - m_TileSize) {
		m_Position.y = m_Arena.height - m_TileSize;
	}

	if (m_Position.y < m_Arena.top + m_TileSize) {
		m_Position.y = m_Arena.top + m_TileSize;
	}

	// Calculer l'angle de rotation du joueur
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2) 
		* 180) / 3.141;

	m_Sprite.setRotation(angle);
}

// SpeedBoost
void Player::upgradeSpeed() {
	// 20% vitesse en plus
	m_Speed += (START_SPEED * .2);
}

// Améliorer la santé
void Player::upgradeHealth() {
	// 20% de santé en plus
	m_MaxHealth += (START_HEALTH * .2);
}

// Augmenter la taille du chargeur
void Player::increaseHealthLevel(int amount) {
	m_Health += amount;

	// Mais pas au-delà du maximum
	if (m_Health > m_MaxHealth) {
		m_Health = m_MaxHealth;
	}
}