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

	// Associate a texture with the sprite
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player.png"));
	// Set the origin of the sprite to the center,
	// for smooth rotation
	m_Sprite.setOrigin(25, 25);
}

// Spawn the player in the middle of the arena
void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) {
	// Place the player in the middle of the arena
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	// Copy the details of the arena to the player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	// Remember how big the tiles are in this arena
	m_TileSize = tileSize;

	// Store the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

// Reset the player's stats at the start of a new game
void Player::resetPlayerStats() {
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

// Last hit time
Time Player::getLastHitTime() {
	return m_LastHit;
}

// Handle the player getting hit by a zombie
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

// Where is the player
FloatRect Player::getPosition() {
	return m_Sprite.getGlobalBounds();
}

// Where is the center of the player
Vector2f Player::getCenter() {
	return m_Position;
}

// What angle is the player facing
float Player::getRotation() {
	return m_Sprite.getRotation();
}

// Send a copy of the sprite to main
Sprite Player::getSprite() {
	return m_Sprite;
}

// Get the player's current health
int Player::getHealth() {
	return m_Health;
}

// Move the player
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

// Stop the player moving in a specific direction
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

// We will call this function once every frame
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
	
	// Keep the player in the arena
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

	// Calculate the angle the player is facing
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2) 
		* 180) / 3.141;

	m_Sprite.setRotation(angle);
}

// Give the player a speed boost
void Player::upgradeSpeed() {
	// 20% speed upgrade
	m_Speed += (START_SPEED * .2);
}

// Give the player some health
void Player::upgradeHealth() {
	// 20% max health upgrade
	m_MaxHealth += (START_HEALTH * .2);
}

// Increase the maximum amount of health the player can have
void Player::increaseHealthLevel(int amount) {
	m_Health += amount;

	// But not beyond the maximum
	if (m_Health > m_MaxHealth) {
		m_Health = m_MaxHealth;
	}
}