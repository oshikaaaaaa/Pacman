#pragma once

#include <SFML/Graphics.hpp>

class SnowLayer {
public:
    SnowLayer(const sf::Vector2f& position, const std::string& snowTexturePath);

    void draw(sf::RenderWindow& window);

private:
    sf::Sprite snowSprite;
};

