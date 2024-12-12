#include "inputField.hpp"
#include "../helpers/initializeText.hpp"

InputField::InputField() {
    m_fieldRect = sf::FloatRect(0, 0, 0, 0);
    m_text = *initializeText("", sf::Vector2f(0 + 12, 0 + 12), 0, sf::Color::Black);
    m_string = "";
    m_selected = false;
}

InputField::InputField(const float& rectLeft, const float& rectTop, const float& rectWidth, 
                       const float& rectHeight, const sf::Color& textColour) {
    
    m_fieldRect = sf::FloatRect(rectLeft, rectTop, rectWidth, rectHeight);
    m_text = *initializeText("", sf::Vector2f(rectLeft + 12, rectTop + 12), rectHeight, textColour);
    m_string = "";
    m_selected = false;
}

sf::FloatRect& InputField::getFieldRect() {
    return m_fieldRect;
}

sf::Text& InputField::getText() {
    return m_text;
}

void InputField::setText(std::string text) {
    m_string = text;
    m_text.setString(text);
}

std::string InputField::getString() {
    return m_string;
}

bool& InputField::getSelected() {
    return m_selected;
}

void InputField::setSelected(bool selected) {
    m_selected = selected;
}

bool InputField::clickedWhileInactive(sf::Vector2f mousePosition) {
    return !m_selected && m_fieldRect.contains(mousePosition);
}

bool InputField::clicked(sf::Vector2f mousePosition) {
    return m_fieldRect.contains(mousePosition);
}

void InputField::render(sf::RenderWindow& window) {
    window.draw(m_text);
}
