#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include"Sound.h"
#include <iostream>

#include "Global.h"
#include "Pacman.h"
#include "MapCollision.h"
#include<fstream>
#include<string>
SoundManager Sound;

using namespace std;




Pacman::Pacman() :
	animation_over(0),
	dead(0),
	direction(0),
	score(0),

	energizer_timer(0),
	position({ 0, 0 }),
	jumping(false),
	jumpForce(0.01f), // Adjust this value as needed
	verticalVelocity(0.0f),
	freeze(false),
	initialPosition({ 160,240 })
{
	//I just realized that I already explained everything in the Ghost class.
	//And I don't like to repeat myself.
	
	ifstream inputFile("highScore.txt");
	if (!inputFile.is_open()) {
		cout << "Error opening the file." << endl;
		
	}
	else {
		inputFile >> highScore;
		
	}
	inputFile.close();
	
}


bool Pacman::get_animation_over()
{
	return animation_over;
}

bool Pacman::get_dead()
{
	return dead;
}

unsigned char Pacman::get_direction()
{
	return direction;
}

unsigned short Pacman::get_energizer_timer()
{
	return energizer_timer;
}
void Pacman::setFrozen() {
	freeze = true;
}

void Pacman::unfreeze() {
	freeze = false;
}

bool Pacman::getFreeze() {
	return freeze;
}


void Pacman::draw_death_animation(sf::RenderWindow& i_window) {
	sf::Sprite sprite;
	sf::Texture texture;

	// Calculate the current frame based on the animation timer
	unsigned char frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));

	sprite.setPosition(position.x, position.y);

	// Load death animation texture
	texture.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");

	// Set sprite texture and texture rect
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));

	// Draw the current frame
	i_window.draw(sprite);

	// Update the animation timer and check if animation is complete
	animation_timer++;

	if (animation_timer >= PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED) {
		animation_timer = 0; // Reset animation timer
		animation_over = true; // Mark animation as complete
	}
}

void Pacman::pacman_sprite(sf::RenderWindow& i_window) {
	sf::Sprite sprite;
	sf::Texture texture;
	sprite.setPosition(initialPosition.x, initialPosition.y);
	if (!texture.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png", sf::IntRect(0,0,16,16)))
	{
		std::cout << "cannot load pacman file";
	}


	sprite.setTexture(texture);
	i_window.draw(sprite);
	

}

void Pacman::draw(bool i_victory, sf::RenderWindow& i_window, bool frozen)
{
	unsigned char frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));

	sf::Sprite sprite;
	sf::Texture texture;

	sprite.setPosition(position.x, position.y);
	/*if (frozen) {
		
		// Draw frozen Pac-Man sprite
		sf::Sprite sprite;
		sf::Texture texture;

		sprite.setPosition(position.x, position.y);

		texture.loadFromFile("Resources/Images/Pacman" + std::to_string(CELL_SIZE) + ".png"); // Replace with the path to your frozen Pac-Man sprite
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, CELL_SIZE, CELL_SIZE));;
		
		
		i_window.draw(sprite);
	
	}
	*/

	

	 if (1 == dead || 1 == i_victory)
	{
		if (animation_timer < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
		{
			animation_timer++;

			texture.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");

			sprite.setTexture(texture);
			sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));

			i_window.draw(sprite);
		}
		else
		{
			// Determine how many times to repeat the animation
			int numAnimationLoops = 3; // Change this value as needed

			// Calculate the current animation frame within a loop
			unsigned char loopedFrame = static_cast<unsigned char>(frame % PACMAN_DEATH_FRAMES);

			// Draw the repeated frames of the death animation
			texture.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");
			sprite.setTexture(texture);

			for (int i = 0; i < numAnimationLoops; ++i)
			{
				sprite.setTextureRect(sf::IntRect(CELL_SIZE * loopedFrame, 0, CELL_SIZE, CELL_SIZE));
				i_window.draw(sprite);
			}

			// Animation is now over after repeating
			animation_over = true;
		}
	}
	else
	{
		// Draw regular Pac-Man sprite
		texture.loadFromFile("Resources/Images/Pacman" + std::to_string(CELL_SIZE) + ".png");
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));
		i_window.draw(sprite);

		animation_timer = (1 + animation_timer) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
	}
}

void Pacman::reset()
{
	animation_over = 0;
	dead = 0;

	direction = 0;

	animation_timer = 0;
	energizer_timer = 0;
	position.x = initialPosition.x;
	position.y = initialPosition.y;
	

}

void Pacman::set_animation_timer(unsigned short i_animation_timer)
{
	animation_timer = i_animation_timer;
}

void Pacman::set_dead(bool i_dead)
{
	dead = i_dead;

	if (1 == dead)
	{
		//Making sure that the animation starts from the beginning.
		animation_timer = 0;
	}
}

void Pacman::set_position(short i_x, short i_y)
{
	position = { i_x, i_y };
}

void Pacman::update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, unsigned delta_time,bool freeze)
{

	std::array<bool, 4> walls{};
	walls[0] = map_collision(0, 0, PACMAN_SPEED + position.x, position.y, i_map, *this);
	walls[1] = map_collision(0, 0, position.x, position.y - PACMAN_SPEED, i_map, *this);
	walls[2] = map_collision(0, 0, position.x - PACMAN_SPEED, position.y, i_map, *this);
	walls[3] = map_collision(0, 0, position.x, PACMAN_SPEED + position.y, i_map, *this);

		


		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (0 == walls[0]) //You can't turn in this direction if there's a wall there.
			{
				direction = 0;
			}
		}

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (0 == walls[1])
			{
				direction = 1;
			}
		}

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if (0 == walls[2])
			{
				direction = 2;
			}
		}


		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (0 == walls[3])
			{
				direction = 3;
			}


		}
	




	short ground_y = position.y;
	while (!map_collision(false, false, position.x, ground_y + 1, i_map, *this))
	{
		ground_y++;
	}
	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !jumping) {
		jumping = true;
		verticalVelocity = jumpForce;
	}

	// Jumping logic
	if (jumping) {
		// Update vertical velocity due to gravity
		verticalVelocity += GRAVITY * delta_time;

		// Update position based on vertical velocity
		//position.y += verticalVelocity;

		// Check for collision with ground
		if (position.y >= ground_y) {
			position.y = ground_y;
			jumping = false;
			verticalVelocity = 0.0f;
		}

	}
		


	


	if (0 == walls[direction])
	{
		switch (direction)
		{
		case 0:
		{
			position.x += PACMAN_SPEED;

			break;
		}
		case 1:
		{
			position.y -= PACMAN_SPEED;

			break;
		}
		case 2:
		{
			position.x -= PACMAN_SPEED;

			break;
		}
		case 3:
		{
			position.y += PACMAN_SPEED;
		}


		}
	}

	if (-CELL_SIZE >= position.x)
	{
		position.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED;
	}
	else if (CELL_SIZE * MAP_WIDTH <= position.x)
	{
		position.x = PACMAN_SPEED - CELL_SIZE;
	}

	if (1 == map_collision(1, 0, position.x, position.y, i_map, *this)) //When Pacman eats an energizer...
	{
		//He becomes energized!
		energizer_timer = static_cast<unsigned short>(ENERGIZER_DURATION / pow(2, i_level));
		
		Sound.eatGhost();
	}
	else
	{
		energizer_timer = std::max(0, energizer_timer - 1);
	}
}

Position Pacman::get_position()
{
	return position;
}

int& Pacman:: Score(){
	return score;
}

int& Pacman::Score(int x) {
	score += x;
	return score;
}

int Pacman:: updateHighScore(int x) {
	highScore = x;
	return highScore;
	
}

int Pacman::getHighScore() {
	return highScore;

}

void Pacman::getS() {
	cout << s;
}

