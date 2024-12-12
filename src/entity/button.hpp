#pragma once

#include <SFML/Window/Cursor.hpp>
#include "../helpers/playSound.hpp"

class Button {
    public:
        Button(const sf::Vector2f& positionVector, const sf::Vector2f& scaleVector, 
               const sf::Texture& idleTexture, const sf::Texture& activeTexture, sf::RenderWindow& window);
        
        void render() const;

        bool getButtonState() const;
        void updateButtonState(sf::Vector2f mousePosition);

    private:
        const sf::Texture* m_idleTexture;
        const sf::Texture* m_activeTexture;
        sf::Sprite* m_sprite;
        sf::RenderWindow& m_window;

        bool active = false;
};
