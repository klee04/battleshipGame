#include "miniGridCell.hpp"

MiniGridCell::MiniGridCell() {
    m_gridPosition.first = -1;
    m_gridPosition.second = -1;
    m_bottomSprite = nullptr;
    m_topSprite = nullptr;
}

MiniGridCell::MiniGridCell(const int& firstGridPosition, const int& secondGridPosition, 
 const sf::Vector2f& globalPosition) {
    m_gridPosition.first = firstGridPosition;
    m_gridPosition.second = secondGridPosition;

    m_bottomSprite = new sf::Sprite();
    m_bottomSprite->setPosition(globalPosition);
    m_bottomSprite->setScale(sf::Vector2f(4,4));

    m_topSprite = new sf::Sprite();
    m_topSprite->setPosition(globalPosition);
    m_topSprite->setScale(sf::Vector2f(4,4));
}

MiniGridCell::~MiniGridCell() {
    delete m_bottomSprite;
    m_bottomSprite = nullptr;

    delete m_topSprite;
    m_topSprite = nullptr;
}

void MiniGridCell::setBottomTexture(const int& textureIndex) {
    m_bottomSprite->setTexture(
        *ResourceManager::getTexture(m_texturePaths[textureIndex])
    );
}


void MiniGridCell::setTopTexture(const int& textureIndex) {
    m_topSprite->setTexture(
        *ResourceManager::getTexture(m_texturePaths[textureIndex])
    );
}

void MiniGridCell::render(sf::RenderWindow& window) const {
    if (m_bottomSprite->getTexture() != nullptr)
        window.draw(*m_bottomSprite);
    if (m_topSprite->getTexture() != nullptr)
        window.draw(*m_topSprite);
}

