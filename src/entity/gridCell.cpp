#include "gridCell.hpp"

GridCell::GridCell() {
    m_gridPosition.first = -1;
    m_gridPosition.second = -1;
    m_rect = nullptr;
}

GridCell::GridCell(const int& firstGridPosition, const int& secondGridPosition, 
 const sf::Vector2f& globalPosition, const bool spriteNeeded) {
    m_gridPosition.first = firstGridPosition;
    m_gridPosition.second = secondGridPosition;
    m_rect = new sf::FloatRect(globalPosition.x, globalPosition.y, 40, 40);

    if (spriteNeeded) {
        m_sprite = new sf::Sprite();
        m_sprite->setPosition(getPosition() + sf::Vector2f(4, 0));
        m_sprite->setScale(sf::Vector2f(4,4));

        m_hitMarker = new sf::Sprite();
        m_hitMarker->setPosition(getPosition() + sf::Vector2f(4, 0));
        m_hitMarker->setScale(sf::Vector2f(4,4));
        m_hitMarker->setTexture(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::HitMarker])
        );
    }
}

GridCell::~GridCell() {
    delete m_rect;
    m_rect = nullptr;

    if (m_sprite != nullptr) {
        delete m_sprite;
        m_sprite = nullptr;
    }

    if (m_hitMarker != nullptr) {
        delete m_hitMarker;
        m_hitMarker = nullptr;
    }
}

bool GridCell::isInsideRect(const sf::Vector2f& position) {
    return m_rect->contains(position);
}

sf::Vector2f GridCell::getPosition() {
    return sf::Vector2f(m_rect->left, m_rect->top);
}

void GridCell::setSpriteTexture(const int& textureIndex) {
    m_sprite->setTexture(
        *ResourceManager::getTexture(m_texturePaths[textureIndex])
    );

    spritePlaced = true;
}

void GridCell::updateHitMarker(const sf::Vector2f& mousePosition, const bool& hitAttemptMade) {
    if (isInsideRect(mousePosition) && !spritePlaced && !hitAttemptMade) {
        showHitMarker = true;
    }
    else
        showHitMarker = false;
}

void GridCell::render(sf::RenderWindow& window) const {
    if (m_sprite->getTexture() != nullptr)
        window.draw(*m_sprite);
    
    if (showHitMarker)
        window.draw(*m_hitMarker);
}
