#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup(int type) {
	// Garder le type de pickup
	m_Type = type;

	// Assciation des texture dans un sprite
	if (m_Type == 1)
	{
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/health_pickup.png"));

		// Valeur du kit de soin
		m_Value = HEALTH_START_VALUE;
	}
	else
	{
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/ammo_pickup.png"));

		// Valeur des munitions
		m_Value = AMMO_START_VALUE;
	}

	// Set l'origine du sprite au centre de l'aréne 
	m_Sprite.setOrigin(25, 25);

	m_SecondsToLive = START_SECONDS_TO_LIVE;
	m_SecondsToWait = START_WAIT_TIME;
}

void Pickup::setArena(IntRect arena)
{
	// Copie les detail de l'arene dans m_Arena
	m_Arena.left = arena.left + 50;
	m_Arena.width = arena.width - 50;
	m_Arena.top = arena.top + 50;
	m_Arena.height = arena.height - 50;

	spawn();
}

void Pickup::spawn()
{
	// Spawn les pickup dans l'arene aléatoirement 
	srand((int)time(0) / m_Type);
	int x = (rand() % m_Arena.width);
	srand((int)time(0) * m_Type);
	int y = (rand() % m_Arena.height);

	m_SecondsSinceSpawn = 0;
	m_Spawned = true;

	m_Sprite.setPosition(x, y);
}

FloatRect Pickup::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Pickup::getSprite()
{
	return m_Sprite;
}

bool Pickup::isSpawned()
{
	return m_Spawned;
}

int Pickup::gotIt()
{
	m_Spawned = false;
	m_SecondsSinceDeSpawn = 0;
	return m_Value;
}

void Pickup::update(float elapsedTime)
{
	if (m_Spawned)
	{
		m_SecondsSinceSpawn += elapsedTime;
	}
	else
	{
		m_SecondsSinceDeSpawn += elapsedTime;
	}
	
	// Si le pickup est spawn et que le temps de vie est dépassé
	if (m_SecondsSinceSpawn > m_SecondsToLive && m_Spawned)
	{
		// Le pickup disparait et le fait respawn dans une autre location
		m_Spawned = false;
		m_SecondsSinceDeSpawn = 0;
	}

	// Si aucun pickup et spawn
	if (m_SecondsSinceDeSpawn > m_SecondsToWait && !m_Spawned)
	{
		// Faire spawn les pickup et reset le timer
		spawn();
	}
}

void Pickup::upgrade()
{
	// Amélioration des pickup
	if (m_Type == 1)
	{
		m_Value += (HEALTH_START_VALUE * .5);
	}
	else
	{
		m_Value += (AMMO_START_VALUE * .5);
	}

	// Plus de temps pour récupérer les pickup avant leur disparition
	m_SecondsToLive += (START_SECONDS_TO_LIVE / 10);
	m_SecondsToWait -= (START_WAIT_TIME / 10);
}