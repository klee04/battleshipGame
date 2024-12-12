#include "inputFieldBar.hpp"

int m_frameCounter = 0;

InputFieldBar::InputFieldBar() {
    sf::RectangleShape* inputFieldBar = new sf::RectangleShape();
    inputFieldBar->setSize(sf::Vector2f(5, 8*4));
    inputFieldBar->setFillColor(sf::Color::Transparent);
    m_inputFieldBar = inputFieldBar;
}

void InputFieldBar::updateInputFieldBarPosition(sf::Text& text, sf::FloatRect& fieldRect) {
    sf::FloatRect globalBounds = text.getGlobalBounds();
    m_inputFieldBar->setPosition(sf::Vector2f(
        globalBounds.left + globalBounds.width + 1, 
        fieldRect.top + 12
    ));

    m_frameCounter = 0;

    if (m_inputFieldBar->getFillColor() == sf::Color::Transparent) 
        m_inputFieldBar->setFillColor(sf::Color::Black);
}

sf::Color InputFieldBar::getColour() {
    return m_inputFieldBar->getFillColor();
}

void InputFieldBar::setColour(sf::Color colour) {
    m_inputFieldBar->setFillColor(colour);
}

void InputFieldBar::animate() {
    if (m_frameCounter >= 15) {
        if (getColour() == sf::Color::Black)
            setColour(sf::Color::Transparent);
        else if (getColour() == sf::Color::Transparent)
            setColour(sf::Color::Black);
        
        m_frameCounter = 0;
    } else m_frameCounter++;
}

void InputFieldBar::render(sf::RenderWindow& window) const {
    window.draw(*m_inputFieldBar);
}