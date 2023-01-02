// HelloSFML.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

using namespace sf;
using namespace std;

int main() 
{
	// Instancier un objet de la classe TextureHolder
	TextureHolder holder;
	
	// 4 différents états du jeu
	enum class State{ PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// Le jeu commence dans l'état GAME_OVER
	State state = State::GAME_OVER;
	
	// Résolution de la fenêtre
	Vector2f resolution;
	resolution.x = 800;
	resolution.y = 800;
	
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena");
	
	// Création de la vue principale
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	
	// Creation d'un objet Clock pour mesurer le temps
	Clock clock;
	// Temps écoulé depuis que le jeu est dans l'état PLAYING
	Time gameTimeTotal;

	// Ou ce trouve la souris par rapport aux coordonnées du monde
	Vector2f mouseWorldPosition;
	// Ou ce trouve la souris par rapport aux coordonnées de la fenêtre
	Vector2i mouseScreenPosition;

	// Création d'un objet Player
	Player player;
	
	// Bordure de l'arene
	IntRect arena;

	// Création d'un background
	VertexArray background;
	// Load la texture du background
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");
	
	// Variables pour le nombre de zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;
	
	// Variable pour le nombre de munitions
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	// Quand le joueur a t'il tiré la dernière fois ?
	Time lastPressed;
	
	// Cacher le pointeur de la souris et le remplacer par un curseur
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");

	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Associer les pickups avec un type
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// initialisation du scord et du meilleur score
	int	score = 0;
	int hiScore = 0;

	// Pour la fenetre d'acceuil et de game over
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	
	// HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Icon de munition
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 490);

	// Load font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	// Pause
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(40);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(350, 200);
	pausedText.setString("Press Enter \nto continue");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(40);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(280, 540);
	gameOverText.setString("Press Enter to play");

	// Niveau augmenter
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(40);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	// munition
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(28);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 480);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(28);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10, 0);
	
	// CHarger le fichier du meilleur score
	ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	// Meilleur score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(25);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(480, 0);
	stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Enemie manquand
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(22);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(975, 310);
	zombiesRemainingText.setString("Zombies: 100");

	// Vague enemie
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(25);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(20, 310);
	waveNumberText.setString("Wave: 0");

	// Bare de vie 
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(150, 310);
	
	// QUand on a update le HUD la dernier foi
	int framesSinceLastHUDUpdate = 0;

	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	// Préparer le son toucher des énemis
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Préparer le son des mort des énemis
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);

	// Préparer le son de tir
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// Préparer le son de recharge
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	// Préparer le son de recharge loupé
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);
	
	// Préparer le son de power up
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// Préparer le son des pickup
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);
		
	// main game loop
	while (window.isOpen()) {
		
		/*
		************************ 
		Gestion des input
		************************
		*/ 
		
		Event event;
		while (window.pollEvent(event))
		{
			// Pause le jeux quand on appuie sur la touche Entré
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart quand le jeu et en pause
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset la cloque pour qu'il n'y a pas de bon d'image
					clock.restart();
				}

				// Lancer une nouvelle partie si le jeux et dans l'état GAME_OVER
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
					wave = 0;
					score = 0;

					// Prépare l'arme et les munitions pour la nouvelle partie
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					// Reset le joueur
					player.resetPlayerStats();
				}
				
				if (state == State::PLAYING)
				{
					// Reloading
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							// On a assez de munitions pour recharger le chargeur
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						else if (bulletsSpare > 0)
						{
							// Balle restante dans le chargeur
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else
						{
							// Rchargement impossible
							reloadFailed.play();
						}
					}
				}
			}
		}
		
		// Gerer les input pour quitter le jeu
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		
		// Gerer les input quand on est en cour de jeu 
		if (state == State::PLAYING){
			// Gestion des input pour le déplacement du joueur ZQSD
			if (Keyboard::isKeyPressed(Keyboard::Z))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}
			
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::Q))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}

			// Tirer une balle
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					//Passer le centre du joueur et le centre du crosshair dans la fonction shoot
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					
					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					
					shoot.play();

					bulletsInClip--;
				}
			} 

		}
		
		// Gerer l'état LEVELING_UP
		if (state == State::LEVELING_UP)
		{
			// Gerer les input pour augmenter les stats du joueur
			if (event.key.code == Keyboard::Num1)
			{
				// augmenter la cadence de tire
				fireRate++;
				state = State::PLAYING;
			}
			
			if (event.key.code == Keyboard::Num2)
			{
				// Augmenter la capaciter du chargeur
				clipSize += clipSize;
				state = State::PLAYING;
			}
			
			if (event.key.code == Keyboard::Num3)
			{
				// Augmenter la vie du joueur
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				// Augmenter la vitesse du joueur
				player.upgradeSpeed();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				// Augmenter le taux de soin
				healthPickup.upgrade();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				// Augmenter le taux de munitions
				ammoPickup.upgrade();
				state = State::PLAYING;
			}
			
			if (state == State::PLAYING)
			{
				// Augmenter le nombre de vague
				wave++;

				// Preparer le niveau
				arena.width = 500 * wave;
				arena.height = 500* wave;
				arena.left = 0;
				arena.top = 0;
				
				int tileSize = createBackground(background, arena);

				// Faire spawn le joueur au milieu de l'aréne
				player.spawn(arena, resolution, tileSize);

				// Configurer les pickup
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				// Crée un horde d'énemie
				numZombies = 5 * wave;

				// Suprimer les précédent mémoire loué
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				// Jouer le son du power up
				powerup.play();
				
				// Reset la clock pour eviter les saut d'image
				clock.restart();
			}
		}

		/*
		*********************
		Mettre a jour les frame
		**********************
		*/

		if (state == State::PLAYING) {
			// Metre a jour le delta time
			Time dt = clock.restart();
			// Mettre a jour le temps total du jeu
			gameTimeTotal += dt;
			// Fraction decimale de 1 sur le delta time
			float dtAsSeconds = dt.asSeconds();
			
			// Ou est pointer la souris 
			mouseScreenPosition = Mouse::getPosition();
			
			// Convertir la position de la souris en coordonné du monde
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
			
			// Mettre le crosshair a la position de la souris
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Mettre a jour le joueur 
			player.update(dtAsSeconds, Mouse::getPosition());
			
			// Prendre en note la nouvelle position du joueur 
			Vector2f playerPosition(player.getCenter());
			
			// Mettre la vue principale au centre du joueur
			mainView.setCenter(player.getCenter());

			// Bovule pour chaque énemis
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			// Mettre a jour les balles 
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			//	Mettre a jour les pickup
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);
	
			// Deterction des collision
			// Est ce que l'énemis a été touché
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							// Stop de la balle
							bullets[i].stop();

							// Enregitrer le point de contact et verifier si l'enemis est mort
							if (zombies[j].hit())
							{
								// Si mort
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}

								numZombiesAlive--;
								
								// quand les enemis sont mort
								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}
								// jouer le sont de mort
								splat.play();
							}
						}
					}
				}
			} 
			
			// Faire que tout les zombie a toucher le joueur
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						// Jouer le son de toucher
						hit.play();
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
						ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			} 

			//	Est ce que le joueur a toucher un kit de soin
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				// jouer le son du pickup
				pickup.play();
			}

			// Est ce que le joueur a toucher un kit de munition
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
				// Jouer le sond de recharge
				reload.play();
			}

			// Taille de la barre de vie
			healthBar.setSize(Vector2f(player.getHealth() * 1.5, 35));

			// Incrementer le nombre de frames depuis le dernier calcul du HUD
			framesSinceLastHUDUpdate++;
			// Calculer les FPS tou les m_FramesPerHUDUpdate frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) {
				// Mettre a jour le text du HUD
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				std::stringstream ssHealth;
				std::stringstream ssFPS;
				std::stringstream ssMouseX;
				std::stringstream ssMouseY;

				// Mettre à jour les munition
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				// Mettre à jour les score
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// Mettre à jour les meilleur score
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				// Mettre à jour le nombre de la vague
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				// Mettre a jour le nombre d'énemis vivant
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				// Reset la frame
				framesSinceLastHUDUpdate = 0;
			}
			
		} 

		/*
		****************
		Dessiner la scène
		****************
		*/ 
		
		if (state == State::PLAYING)
		{
			window.clear();

			// Dessiner la main view
			window.setView(mainView);

			// Dessiner le background
			window.draw(background, &textureBackground);

			// Desiner les énemis
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			} 

			// Dessiner les balles
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}
			
			// Desinner les joueur
			window.draw(player.getSprite());

			// Deesinner les pick up
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}		
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}

			// Dessiner le crosshair
			window.draw(spriteCrosshair);

			// Changer de view avec le HUD
			window.setView(hudView);

			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
			window.draw(healthBar);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		
		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}
		
		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		
		window.display();
		
	} 
	// Supprimer la mémoire alloué
	delete[] zombies;
	return 0;
}