#include "SnowLayer.h"
#include <iostream>

SnowLayer::SnowLayer(const sf::Vector2f& position, const std::string& snowTexturePath) {
    sf::Texture snowTexture;
    if (!snowTexture.loadFromFile(snowTexturePath)) {
        std::cout << "Failed to load snow texture: " << snowTexturePath << std::endl;
        
    }
    std::cout << "loaded";


    snowSprite.setTexture(snowTexture);
    snowSprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
    snowSprite.setOrigin(snowTexture.getSize().x / 2.0f, snowTexture.getSize().y / 2.0f);
    snowSprite.setPosition(position);
}

void SnowLayer::draw(sf::RenderWindow& window) {
    window.draw(snowSprite);
   
}
