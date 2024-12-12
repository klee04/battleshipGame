#pragma once
#include "../helpers/initializeSprite.hpp"

class Ship {
    public:
        Ship(const sf::Texture& texture, const sf::Vector2f& positionVector,
             const sf::Vector2f& scaleVector);

        sf::Sprite* getSprite();

        bool getHoverState() const;
        void updateHoverState(sf::Vector2f mousePosition);

        const sf::Vector2f& getPosition();
        void setPosition(sf::Vector2f position, const bool& isDragging, 
                         const bool returningToSavedPosition = false);

        const sf::Vector2f& getSavedPosition();
        bool getIsHorizontal();
        bool getSavedIsHorizontal();

        void rotate();

        void render(sf::RenderWindow& window) const;
    private:
        sf::Sprite* m_sprite;
        bool m_hovered;
        bool m_isHorizontal;

        sf::Vector2f m_savedPosition;
        bool m_savedIsHorizontal;
};
