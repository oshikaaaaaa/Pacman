#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>


class SoundManager {
private:
	sf::SoundBuffer pellets;
	sf::SoundBuffer death;
	sf::SoundBuffer ghost;
	sf::SoundBuffer intro;
	sf::Sound Playintro;
	sf::Sound Ghost;
	sf::Sound dead;
	sf::Sound pellet;

public:
	SoundManager();
	void Eat();
	void Dead();
	void eatGhost();
	void playIntro();
	void pauseIntro();
	void MusicIntro();
	bool IsIntroPlaying();
	bool IsPlayingDeathSound();
	bool IsPlayingChompSound();
	bool IsMusicIntroPlaying();
};