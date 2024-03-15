#include <sstream>
#include "Player.h"
#include "ZombieArena.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TextureHolder.h"
#include "Pickup.h"

#include <iostream>
#include <fstream>


using namespace sf;

//GAME_OVER is state where home screen is displayed
static enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
TextureHolder holder;


int main() {
	


	//start with game over state
	State state = State::GAME_OVER;

	Vector2f resolution(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);
	//view for main action
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));
	mainView.zoom(0.8f);
	
	Clock clock;
	Time gameTimeTotal;

	//mouse location
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	Player player;

	//arena bounds
	IntRect arena;

	//create the background
	VertexArray background;
	//Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");
	

	//prepare horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	//prepare the bullets
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	//when was the fire button last pressed?
	Time lastPressed;

	//prepare the reload
	Time reloadInstant;
	int reloadTime = 1000;
	bool reloadInProgress = false;


	//replacing mouse
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");

	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	//pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);


	//Gamescore
	int score = 0;
	int hiScore = 0;

	
	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);
	// Load the font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");
	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");
	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");
	// LEVELING up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
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
	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);
	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	//load high score from text file
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open()) {
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());
	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");
	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");
	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;



	// Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);
	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);
	// Prepare the shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);
	// Prepare the reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);
	// Prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);
	// Prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);
	// Prepare the pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	// TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA
	
	// TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA TEST AREA

	bool enterPressed = false;
	/************************
		GAME LOOP
	*************************/
	while (window.isOpen()) {
		/**************
		handle input
		**************/
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Enter && state == State::PLAYING) {
					state = State::PAUSED;
					enterPressed = true;
				}
				//Resume the game while playing
				if (event.key.code == Keyboard::Enter && state == State::PAUSED && !enterPressed) {
					state = State::PLAYING;
					//reset clock, so no frame jump
					clock.restart();
				}
				//Start a new game while game is GAME_OVER
				else if (event.key.code == Keyboard::Enter && state == State::GAME_OVER) {
					state = State::LEVELING_UP;
					wave = 0;
					score = 0;

					//prepare gun n ammo
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					player.resetPlayerStats();
				}
				if (state == State::PLAYING) {

					//reloading
					if (Keyboard::isKeyPressed(Keyboard::R)) {
						//can start a reload
						if (bulletsSpare > 0 && !reloadInProgress) {
							//start reload countdown
							reloadInstant = gameTimeTotal;
							reloadInProgress = true;
							reload.play();
						}
						else if (bulletsSpare <= 0) {
							reloadFailed.play();
						}
					}
				}
			}
		}//End event polling
		enterPressed = false;

		//handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			std::ofstream outputFile("gamedata/scores.txt");
			outputFile << hiScore;
			outputFile.close();
			window.close();
		}

		//Handle WASD
		if (state == State::PLAYING) {
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.moveUp();
			}
			else {
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.moveDown();
			}
			else {
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.moveLeft();
			}
			else {
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.moveRight();
			}
			else {
				player.stopRight();
			}
			//FIRE
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0 && !reloadInProgress) {
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					++currentBullet;
					if (currentBullet > 99) {
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;

					shoot.play();
					--bulletsInClip;
				}
			}
		}

		//Handle the leveling up
		if (state == State::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) {
				//increase rate of fire
				++fireRate;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2) {
				//increase clip size
				clipSize += clipSize;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3) {
				//increase health
				player.upgradeHealth();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4) {
				//increase speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5) {
				//upgrade pickup
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6) {
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			if (state == State::PLAYING) {
				//Prepare the level
				++wave;
				arena.width = 600 * wave;
				arena.height = 300 * wave;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background,arena);

				player.spawn(arena, resolution, tileSize);

				//configure pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);


				//create horde of zombies
				numZombies = 14 * wave;
				//delete prev alloc mem
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				powerup.play();

				clock.restart();
			}
		}//END leveling up

		/****************
		UPDATE THE FRAME
		****************/

		if (state == State::PLAYING) {
			//update dt
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds(); //deletable?

			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			player.update(dt.asSeconds(), mouseScreenPosition);

			//make note of player's new position
			Vector2f playerPosition(player.getCenter());  //deletable?

			//make the view centre around the player
			mainView.setCenter(player.getCenter());

			//update zombies
			for (int i = 0; i < numZombies; ++i) {
				zombies[i].update(dt.asSeconds(), playerPosition);
			}
			//update bullets
			for (int i = 0; i < 100; ++i) {
				if (bullets[i].isInFlight()) {
					bullets[i].update(dt.asSeconds());
				}
			}

			//update pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			//update crosshair
			spriteCrosshair.setPosition(mouseWorldPosition);

			//COLLISION DETECTION
			//have any zombies been shot?
			for (int i = 0; i < 100; ++i) {
				for (int j = 0; j < numZombies; ++j) {
					if (bullets[i].isInFlight() && zombies[j].isAlive()) {
						if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
							splat.play();
							//stop the bullet
							bullets[i].stop();
							if (zombies[j].hit()) {
								//if killed
								score += 10;
								if (score >= hiScore) {
									hiScore = score;
								}
								--numZombiesAlive;
								//when all zombies dead
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			} //end zombie shot
			
			//Have any zombie touched the player
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects
				(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						hit.play();
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			}// End player touched

			//update reload status
			if (gameTimeTotal.asMilliseconds() - reloadInstant.asMilliseconds() > reloadTime && reloadInProgress) {
				reloadInProgress = false;
				if ((bulletsSpare + bulletsInClip) > clipSize) {
					bulletsSpare -= clipSize - bulletsInClip;
					bulletsInClip = clipSize;
				}
				else if (bulletsSpare > 0) {
					bulletsInClip += bulletsSpare;
					bulletsSpare = 0;
				}
			}

			//pickup collision
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
				player.increaseHealthLevel(healthPickup.gotIt());
				pickup.play();
			}
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
				bulletsSpare += ammoPickup.gotIt();
				reload.play();
			}

			//size up the healthbar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			++framesSinceLastHUDUpdate;

			// re-calculate every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				// Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				// Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());
				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());
				// Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());
				// Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());
				// Update the high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				framesSinceLastHUDUpdate = 0;
			}// End HUD update
		}



		/**************
		DRAW THE SCENE
		***************/
		if (state == State::PLAYING) {
			window.clear();

			window.setView(mainView);
			//draw the background
			window.draw(background, &textureBackground);
			
			
			//draw zombies dead (again)
			for (int i = 0; i < numZombies; ++i) {
				if (!zombies[i].isAlive()) {
					window.draw(zombies[i].getSprite());
				}
			}
			//draw zombies alive
			for (int i = 0; i < numZombies; ++i) {
				if (zombies[i].isAlive()) {
					window.draw(zombies[i].getSprite());
				}
			}


			//draw the bullets
			for (int i = 0; i < 100; ++i) {
				if(bullets[i].isInFlight())
					window.draw(bullets[i].getShape());
			}

			window.draw(player.getSprite());

			window.draw(spriteCrosshair);

			if (ammoPickup.isSpawned()) {
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned()) {
				window.draw(healthPickup.getSprite());
			}

			//Switch to HUD view
			window.setView(hudView);
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);

		}
		if (state == State::LEVELING_UP) {
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::GAME_OVER) {
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		if (state == State::PAUSED) {
			window.draw(pausedText);
		}

		window.display();

	}//END game loop

	delete[] zombies;

	return 0;
}