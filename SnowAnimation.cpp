#include "SnowAnimation.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

SnowAnimation::SnowAnimation(int windowWidth, int windowHeight, int numSnowflakes)
    : windowWidth(windowWidth), windowHeight(windowHeight) {
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < numSnowflakes; ++i) {
        Snowflake snowflake;
        snowflake.x = static_cast<float>(rand() % windowWidth);
        snowflake.y = static_cast<float>(rand() % windowHeight);
        snowflake.speed = static_cast<float>(rand() % 5) + 1; // Vary the falling speed
        snowflakes.push_back(snowflake);
    }
}

void SnowAnimation::update(float deltaTime) {
    for (auto& snowflake : snowflakes) {
        snowflake.y += snowflake.speed * deltaTime;
        std::cout << "snow\n";

        if (snowflake.y > windowHeight) {
            snowflake.y = 0;
            snowflake.x = static_cast<float>(rand() % windowWidth);
        }
    }
}

void SnowAnimation::draw(sf::RenderWindow& window) {
    for (const auto& snowflake : snowflakes) {
        sf::CircleShape shape(2); // Snowflake is represented by a small circle
        shape.setPosition(snowflake.x, snowflake.y);
        shape.setFillColor(sf::Color::White);
        window.draw(shape);
    }
}
