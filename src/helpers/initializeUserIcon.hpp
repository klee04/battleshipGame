#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

inline sf::CircleShape* initializeUserIcon(const sf::Vector2f& positionVector, const float& radius, 
                                           const sf::Color& colour) {
    sf::CircleShape* icon = new sf::CircleShape(radius, 15UL);
    icon->setPosition(positionVector);
    icon->setFillColor(colour);
    icon->setOutlineColor(sf::Color::Black);
    icon->setOutlineThickness(5);
    return icon;
}
