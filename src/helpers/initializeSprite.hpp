#pragma once
#include <SFML/Graphics.hpp>

inline sf::Sprite* initializeSprite(const sf::Texture& texture, const sf::Vector2f& positionVector,
                                    const sf::Vector2f& scaleVector) {
    sf::Sprite* sprite = new sf::Sprite();
    sprite->setTexture(texture);
    sprite->setPosition(positionVector);
    sprite->setScale(scaleVector);
    return sprite;
}
