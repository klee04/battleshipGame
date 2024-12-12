#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class InputFieldBar {
    public:
        InputFieldBar();

        void updateInputFieldBarPosition(sf::Text& text, sf::FloatRect& fieldRect);

        sf::Color getColour();
        void setColour(sf::Color colour);

        void animate();

        void render(sf::RenderWindow& window) const;

    private:
        sf::RectangleShape* m_inputFieldBar;
        int m_frameCounter = 0;
};
