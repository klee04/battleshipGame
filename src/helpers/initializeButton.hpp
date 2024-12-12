#pragma once
#include <SFML/Graphics.hpp>
#include "../entity/button.hpp"

inline Button* initializeButton(const sf::Vector2f& positionVector, const sf::Vector2f& scaleVector, 
                                const sf::Texture& idleTexture, const sf::Texture& activeTexture, 
                                sf::RenderWindow& window) {
    return new Button(positionVector, scaleVector, idleTexture, activeTexture, window);
}
