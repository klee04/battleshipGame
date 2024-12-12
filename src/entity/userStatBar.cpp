#include "userStatBar.hpp"
#include "../helpers/initializeSprite.hpp"
#include "../helpers/initializeText.hpp"
#include "../helpers/initializeUserIcon.hpp"

UserStatBar::UserStatBar(const sf::Vector2f& positionVector, const sf::Vector2f& scaleVector) {
    m_sprite = initializeSprite(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::UserStatBarTexture]),
        positionVector,
        scaleVector
    );
}

UserStatBar::UserStatBar(const sf::Vector2f& positionVector, const sf::Vector2f& scaleVector, 
 const int& rankingNumber, const int& userIconColour, const std::string& username, 
 const int& statToDisplay, const int& numberToDisplay, sf::RenderWindow& window, 
 const bool currentUser) {

    if (currentUser) {
        m_sprite = initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::CurrentUserStatBarTexture]),
            positionVector,
            scaleVector
        );
    } else {
        m_sprite = initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::UserStatBarTexture]),
            positionVector,
            scaleVector
        );
    }

    // Initialize text & user icon
    m_rankingNumberText = initializeText(
        std::to_string(rankingNumber) + ".",
        sf::Vector2f(m_sprite->getGlobalBounds().left, m_sprite->getGlobalBounds().top)
 + sf::Vector2f(7*4, 4*4),
        20 * 4,
        sf::Color::White
    );

    m_userIcon = initializeUserIcon(
        sf::Vector2f(m_sprite->getGlobalBounds().left, m_sprite->getGlobalBounds().top)
 + sf::Vector2f(24*4, 4*4),
        6 * 4,
        m_iconColours[userIconColour]
    );

    m_usernameText = initializeText(
        username,
        sf::Vector2f(m_sprite->getGlobalBounds().left, m_sprite->getGlobalBounds().top) + sf::Vector2f(42*4, 4*4),
        20 * 4,
        sf::Color::White
    );

    m_statText = initializeText(
        m_textToDisplay[statToDisplay],
        sf::Vector2f(m_sprite->getGlobalBounds().left, m_sprite->getGlobalBounds().top) + sf::Vector2f(150*4, 4*4),
        20 * 4,
        sf::Color::White
    );

    m_numberToDisplayText = initializeText(
        std::to_string(numberToDisplay),
        sf::Vector2f(m_sprite->getGlobalBounds().left, m_sprite->getGlobalBounds().top) + sf::Vector2f(220*4, 1*4),
        30 * 4,
        m_iconColours[userIconColour]
    );

}

UserStatBar::~UserStatBar() {
    delete m_sprite;
    m_sprite = nullptr;
    delete m_rankingNumberText;
    m_rankingNumberText = nullptr;
    delete m_usernameText;
    m_usernameText = nullptr;
    delete m_statText;
    m_statText = nullptr;
    delete m_numberToDisplayText;
    m_numberToDisplayText = nullptr;
    delete m_userIcon;
    m_userIcon = nullptr;
}

void UserStatBar::updateInfo(const int& rankingNumber, const int& userIconColour, const std::string& username,
 const int& statToDisplay, const int& numberToDisplay) {
    m_rankingNumberText->setString(std::to_string(rankingNumber) + ".");
    m_userIcon->setFillColor(m_iconColours[userIconColour]);
    m_usernameText->setString(username);
    m_statText->setString(m_textToDisplay[statToDisplay]);
    m_numberToDisplayText->setString(std::to_string(numberToDisplay));
    m_numberToDisplayText->setFillColor(m_iconColours[userIconColour]);
 }

void UserStatBar::render(sf::RenderWindow& window) const {
    window.draw(*m_sprite);
    if (m_rankingNumberText != nullptr) 
        window.draw(*m_rankingNumberText);
    if (m_userIcon != nullptr)
        window.draw(*m_userIcon);
    if (m_usernameText != nullptr) 
        window.draw(*m_usernameText);
    if (m_statText != nullptr) 
        window.draw(*m_statText);
    if (m_numberToDisplayText != nullptr) 
        window.draw(*m_numberToDisplayText);
}
