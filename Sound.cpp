#include "Sound.h"

using namespace sf;

// Constructor
SoundManager::SoundManager() {
    if (!pellets.loadFromFile("pacman_chomp.wav")) {
        std::cout << "ERROR: sounds/fire.wav didn't load";
    }
    else {
        
        pellet.setBuffer(pellets);
    }

    if (!death.loadFromFile("Death.wav")) {
        std::cout << "ERROR: sounds/explo_asteroid.wav didn't load";
    }
    else {
        
        dead.setBuffer(death);
    }

    if (!ghost.loadFromFile("Ghost.wav")) {
        std::cout << "ERROR: sounds/fire.wav didn't load";
    }
    else {
        Ghost.setBuffer(ghost);

    }

    if (!intro.loadFromFile("pacman_beginning.wav")) {
        std::cout << "ERROR: sounds/fire.wav didn't load";
    }
    else {
        Playintro.setBuffer(intro);

    }

   
}

//Functions
void SoundManager:: Eat() {
    pellet.play();
}

void SoundManager:: Dead(){

    dead.play();
}

void SoundManager::eatGhost() {
   
    Ghost.play();
    


}

void SoundManager::playIntro() {

    Playintro.play();
}

void SoundManager::pauseIntro(){

    Playintro.pause();
}


bool SoundManager::IsIntroPlaying() {
    return Playintro.getStatus() == sf::Sound::Playing;

}
bool SoundManager::IsPlayingDeathSound() {
    return dead.getStatus() == sf::Sound::Playing;
}

bool SoundManager::IsPlayingChompSound() {
    return pellet.getStatus() == sf::Sound::Playing;
}





