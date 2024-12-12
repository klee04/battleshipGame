#pragma once
#include "../controllers/resourceManager.hpp"

inline sf::Text* initializeText(const std::string& textString, const sf::Vector2f textPosition, 
                                const int& textSize = 24, const sf::Color& textColour = sf::Color::White) {
    sf::Text* text = new sf::Text();
    text->setFont(*ResourceManager::getFont("TinyUnicode.ttf"));
    text->setString(textString);
    text->setPosition(textPosition - sf::Vector2f(0, textSize / 1.75));  // Subtracting to account for the font's weird offset
    text->setCharacterSize(textSize);
    text->setFillColor(textColour);
    text->setLineSpacing(0);
    return text;
}
