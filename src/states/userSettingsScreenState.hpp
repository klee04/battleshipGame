#pragma once
#include "../controllers/stateManager.hpp"

class UserSettingsScreenState : public State {
    public:
        UserSettingsScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~UserSettingsScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;
        sf::Text m_usernameText;
        sf::CircleShape m_userIcon;

        int m_iconColourIndex = 0;
        sf::Color m_iconColours[6] = {
            sf::Color::Blue,
            sf::Color::Red,
            sf::Color::Magenta,
            sf::Color::Green, 
            sf::Color::Yellow,
            sf::Color::Cyan
        };

        sf::Color getNextColour();
        sf::Color getPreviousColour();
        void changeIconColour(sf::CircleShape& userIcon, bool rightButton);

        std::vector<std::string> m_texturePaths{
            "UserSettingsScreen/userSettingsBackground.png",
            "UserSettingsScreen/leftButtonIdle.png",
            "UserSettingsScreen/leftButtonActive.png",
            "UserSettingsScreen/rightButtonIdle.png",
            "UserSettingsScreen/rightButtonActive.png",
            "UserSettingsScreen/logoutButtonIdle.png",
            "UserSettingsScreen/logoutButtonActive.png",
            "UserSettingsScreen/backButtonIdle.png",
            "UserSettingsScreen/backButtonActive.png",
        };
        
        enum m_textureNames {
            UserSettingsBackgroundTexture,
            LeftButtonIdleTexture,
            LeftButtonActiveTexture,
            RightButtonIdleTexture,
            RightButtonActiveTexture,
            LogoutButtonIdleTexture,
            LogoutButtonActiveTexture,
            BackButtonIdleTexture,
            BackButtonActiveTexture
        };

        enum m_spriteNames {
            UserSettingsBackground
        };

        enum m_buttonNames {
            LeftButton,
            RightButton,
            LogoutButton,
            BackButton
        };
};
