#include "ship.hpp"

Ship::Ship(const sf::Texture& texture, const sf::Vector2f& positionVector,
           const sf::Vector2f& scaleVector) : m_hovered(false), m_savedPosition(positionVector), 
           m_isHorizontal(false), m_savedIsHorizontal(false) {
    
    m_sprite = initializeSprite(texture, positionVector, scaleVector);
}

sf::Sprite* Ship::getSprite() {
    return m_sprite;
}

bool Ship::getHoverState() const {
    return m_hovered;
}

void Ship::updateHoverState(sf::Vector2f mousePosition) {
    if (m_sprite->getGlobalBounds().contains(mousePosition)) {
        if (!m_hovered) m_hovered = true;
    }
    else {
        if (m_hovered) m_hovered = false;
    }
}

const sf::Vector2f& Ship::getPosition() {
    return m_sprite->getPosition();
}

void Ship::setPosition(sf::Vector2f position, const bool& isDragging, 
 const bool returningToSavedPosition) {
    // Account for whitespace offsets in texture
    if (isDragging) {
        if (m_isHorizontal) 
            position = position + sf::Vector2f(-8, 8);
        else 
            position = position - sf::Vector2f(8, 8);
        
    } else if (!returningToSavedPosition) {
        if (m_isHorizontal) 
            position = position + sf::Vector2f(0, 40);
        
        // Save position if position is being set while non-dragging
        m_savedPosition = position;
        m_savedIsHorizontal = m_isHorizontal;
    }

    m_sprite->setPosition(position);
}

const sf::Vector2f& Ship::getSavedPosition() {
    return m_savedPosition;
}

bool Ship::getIsHorizontal() {
    return m_isHorizontal;
}

bool Ship::getSavedIsHorizontal() {
    return m_savedIsHorizontal;
}

void Ship::rotate() {
    if (m_isHorizontal) {
        m_sprite->setRotation(0);
        m_isHorizontal = false;
    } else {
        m_sprite->setRotation(-90);
        m_isHorizontal = true;
    }
}

void Ship::render(sf::RenderWindow& window) const {
    window.draw(*m_sprite);
}
