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
        snowflake.y = static_cast<float>(rand() % -windowHeight); // Start above the visible area
        snowflake.speed = static_cast<float>(rand() % 10) + 5; // Increase speed range
        snowflakes.push_back(snowflake);
    }
}

void SnowAnimation::update(float deltaTime) {
    for (auto& snowflake : snowflakes) {
        snowflake.y += snowflake.speed * deltaTime;

        if (snowflake.y > windowHeight-200) {
            snowflake.y = static_cast<float>(rand() % -windowHeight); // Reset above the visible area
            snowflake.x = static_cast<float>(rand() % windowWidth);
            accumulatedSnowPositions.push_back(sf::Vector2f(snowflake.x, snowflake.y));
        }
       

    }
}

void SnowAnimation::draw(sf::RenderWindow& window) {

    for (const auto& snowflake : snowflakes) {
        sf::CircleShape shape(2.5); // Snowflake is represented by a small circle
        shape.setPosition(snowflake.x, snowflake.y);
        shape.setFillColor(sf::Color::White);
        window.draw(shape);
    }
    


}

void SnowAnimation::accumulatedSnow(sf::RenderWindow& window)
{
    for (const auto& position : accumulatedSnowPositions) {
        sf::CircleShape shape(snowflakeRadius);
        shape.setPosition(position);
        shape.setFillColor(sf::Color::White);
        window.draw(shape);
    }
}
