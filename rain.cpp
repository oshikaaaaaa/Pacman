#include <SFML/Graphics.hpp>
#include "SnowAnimation.h"
/*
int main() {
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const int NUM_SNOWFLAKES = 200;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snowfall Effect");
    window.setFramerateLimit(60);

    SnowAnimation snowAnimation(WINDOW_WIDTH, WINDOW_HEIGHT, NUM_SNOWFLAKES);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        snowAnimation.update(1.0f / 1.0f); // Pass the elapsed time
        snowAnimation.draw(window);

        window.display();
    }

    return 0;
}
*/