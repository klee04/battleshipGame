#pragma once

#include "../controllers/resourceManager.hpp"
#include <vector>

class UserStatBar {
    public:
        UserStatBar(const sf::Vector2f& positionVector, const sf::Vector2f& scaleVector); 
        UserStatBar(const sf::Vector2f& positionVector, const sf::Vector2f& scaleVector, 
                    const int& rankingNumber, const int& userIconColour, const std::string& username, 
                    const int& statToDisplay, const int& numberToDisplay, sf::RenderWindow& window, 
                    const bool currentUser = false);
        ~UserStatBar();
        
        void render(sf::RenderWindow& window) const;

        void updateInfo(const int& rankingNumber, const int& userIconColour, const std::string& username,
                        const int& statToDisplay, const int& numberToDisplay);

    private:
        sf::Sprite* m_sprite;
        sf::Text* m_rankingNumberText = nullptr;
        sf::Text* m_usernameText = nullptr;
        sf::Text* m_statText = nullptr;
        sf::Text* m_numberToDisplayText = nullptr;
        sf::CircleShape* m_userIcon = nullptr;

        sf::Color m_iconColours[6] = {
            sf::Color::Blue,
            sf::Color::Red,
            sf::Color::Magenta,
            sf::Color::Green, 
            sf::Color::Yellow,
            sf::Color::Cyan
        };

        enum stats {
            TotalWins,
            TotalGamesPlayed
        };
        std::string m_textToDisplay[6] = {
            "TOTAL WINS:",
            "TOTAL GAMES:"
        };

        std::vector<std::string> m_texturePaths{
            "LeaderboardScreen/userStatBar.png",
            "LeaderboardScreen/currentUserStatBar.png"
        };

        enum m_textureNames {
            UserStatBarTexture,
            CurrentUserStatBarTexture
        };
};
