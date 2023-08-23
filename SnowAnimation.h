#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class SnowAnimation {
public:
  
    float snowflakeRadius;
    SnowAnimation(int windowWidth, int windowHeight, int numSnowflakes);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void accumulatedSnow(sf::RenderWindow& window);
    

private:
    struct Snowflake {
        float x, y;
        float speed;
    };

    int windowWidth;
    int windowHeight;
    std::vector<Snowflake> snowflakes;
    std::vector<sf::Vector2f> accumulatedSnowPositions;
};

/*/


class SnowfallAnimation {
public:
    SnowfallAnimation(int windowWidth, int windowHeight, const std::string& texturePath);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float snowYPosition;
    int windowWidth;
    int windowHeight;
    int currentFrame;
    sf::Clock frameClock;
    float frameDuration;
};
*/