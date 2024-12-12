#pragma once
#include "../controllers/stateManager.hpp"

class PlayerTwoLoginScreenState : public State {
    public:
        PlayerTwoLoginScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~PlayerTwoLoginScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        std::vector<std::string> m_texturePaths{
            "Player2LoginScreen/player2LoginBackground.png",
            "Player2LoginScreen/loginButtonIdle.png",
            "Player2LoginScreen/loginButtonActive.png",
            "Player2LoginScreen/guestButtonIdle.png",
            "Player2LoginScreen/guestButtonActive.png",
            "Player2LoginScreen/backButtonIdle.png",
            "Player2LoginScreen/backButtonActive.png"
        };
        
        enum m_textureNames {
            PlayerTwoLoginBackgroundTexture,
            LoginButtonIdleTexture,
            LoginButtonActiveTexture,
            GuestButtonIdleTexture,
            GuestButtonActiveTexture,
            BackButtonIdleTexture,
            BackButtonActiveTexture
        };

        enum m_spriteNames {
            PlayerTwoLoginBackground
        };

        enum m_buttonNames {
            LoginButton,
            GuestButton,
            BackButton
        };
};
