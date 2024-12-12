#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class InputField {
    public:
        InputField();
        
        InputField(const float& rectLeft, const float& rectTop, const float& rectWidth, 
                   const float& rectHeight, const sf::Color& textColour);

        sf::FloatRect& getFieldRect();

        sf::Text& getText();
        void setText(std::string text);

        std::string getString();

        bool& getSelected();
        void setSelected(bool selected);

        bool clickedWhileInactive(sf::Vector2f mousePosition);
        bool clicked(sf::Vector2f mousePosition);

        void render(sf::RenderWindow& window);

    private:
        sf::FloatRect m_fieldRect;
        sf::Text m_text;
        std::string m_string;
        bool m_selected;

};
