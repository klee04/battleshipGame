#pragma once
#include "../controllers/stateManager.hpp"

class MenuScreenState : public State {
    public:
        MenuScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~MenuScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;
        sf::Text* m_usernameText;
        sf::CircleShape* m_userIcon;
        sf::Color m_iconColours[6] = {
            sf::Color::Blue,
            sf::Color::Red,
            sf::Color::Magenta,
            sf::Color::Green, 
            sf::Color::Yellow,
            sf::Color::Cyan
        };

        std::vector<std::string> m_texturePaths{
            "MenuScreen/menuBackground.png",
            "MenuScreen/playVsBotButtonIdle.png",
            "MenuScreen/playVsBotButtonActive.png",
            "MenuScreen/passAndPlayButtonIdle.png",
            "MenuScreen/passAndPlayButtonActive.png",
            "MenuScreen/leaderboardButtonIdle.png",
            "MenuScreen/leaderboardButtonActive.png",
            "MenuScreen/userSettingsButtonIdle.png",
            "MenuScreen/userSettingsButtonActive.png",
            "UserSettingsScreen/logoutButtonIdle.png",
            "UserSettingsScreen/logoutButtonActive.png",
        };
        
        enum m_textureNames {
            MenuBackgroundTexture,
            PlayVsBotButtonIdleTexture,
            PlayVsBotButtonActiveTexture,
            PassAndPlayButtonIdleTexture,
            PassAndPlayButtonActiveTexture,
            LeaderboardButtonIdleTexture,
            LeaderboardButtonActiveTexture,
            UserSettingsButtonIdleTexture,
            UserSettingsButtonActiveTexture,
            LogoutButtonIdleTexture,
            LogoutButtonActiveTexture
        };

        enum m_spriteNames {
            MenuBackground
        };

        enum m_buttonNames {
            PlayVsBotButton,
            PassAndPlayButton,
            LeaderboardButton,
            UserSettingsButton,
            LogoutButton
        };
};