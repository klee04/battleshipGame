#include "button.hpp"

Button::Button(const sf::Vector2f& positionVector, const sf::Vector2f& scaleVector, 
               const sf::Texture& idleTexture, const sf::Texture& activeTexture, sf::RenderWindow& window) 
               : m_window{ window } {
    
    this->m_idleTexture = &idleTexture;
    this->m_activeTexture = &activeTexture;

    sf::Sprite* sprite = new sf::Sprite();
    sprite->setTexture(*this->m_idleTexture);
    sprite->setPosition(positionVector);
    sprite->setScale(scaleVector);
    this->m_sprite = sprite;
}

void Button::render() const {
    sf::Cursor cursor;
    if (active) {
        cursor.loadFromSystem(sf::Cursor::Hand);
        m_window.setMouseCursor(cursor);
    } else {
        cursor.loadFromSystem(sf::Cursor::Arrow);
        m_window.setMouseCursor(cursor);
    }

    m_window.draw(*m_sprite);
}

bool Button::getButtonState() const {
    return active;
}

void Button::updateButtonState(sf::Vector2f mousePosition) {
    if (m_sprite->getGlobalBounds().contains(mousePosition)) {
        // If mouse is on the button
        if (!active) {
            active = true;
            playSound("buttonHover.wav");
            m_sprite->setTexture(*m_activeTexture);
        }
    } else {
        if (active) {
            active = false;
            m_sprite->setTexture(*m_idleTexture);
        }
    }
}
