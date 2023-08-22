#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<cstring>
class Pacman
{
	//This is used for the death animation.
	bool animation_over;
	//Am I dead?
	bool dead;

	unsigned char direction;
	int score;
	int highScore;
	//More timers!
	unsigned short animation_timer;
	unsigned short energizer_timer;
	char s;
	const float GRAVITY = 9.81f;
	bool jumping;
	float jumpForce;
	float verticalVelocity;
	bool freeze;

	//Current location of this creature, commonly known as Pacman.
	Position position;
	Position  initialPosition;
public:
	Pacman();

	bool get_animation_over();
	bool get_dead();

	unsigned char get_direction();

	unsigned short get_energizer_timer();

	void draw(bool i_victory, sf::RenderWindow& i_window, bool frozen);
	void reset();
	void set_animation_timer(unsigned short i_animation_timer);
	void set_dead(bool i_dead);
	void set_position(short i_x, short i_y);
	void update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, unsigned delta_time, bool freeze);
	bool is_death_animation_over() const;
	void draw_death_animation(sf::RenderWindow& window);
	void pacman_sprite(sf::RenderWindow& i_window);
	int &Score();
	int &Score(int x);
	int updateHighScore(int x);
	int getHighScore();
	void getS();
	void clearFile();
	void setFrozen();
	void unfreeze();
	bool getFreeze();

	Position get_position();
};