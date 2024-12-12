#pragma once
#include "../controllers/stateManager.hpp"
#include "../entity/inputFieldBar.hpp"
#include "../entity/inputField.hpp"

class ModifiedLoginScreenState : public State {
    public:
        ModifiedLoginScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~ModifiedLoginScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        InputField m_usernameField;
        InputField m_passwordField;

        InputFieldBar m_inputFieldBar;

        sf::Text* m_errorText;

        std::vector<std::string> m_texturePaths{
            "LoginScreen/modifiedLoginBackground.png",
            "LoginScreen/loginButtonIdle.png",
            "LoginScreen/loginButtonActive.png",
            "LeaderboardScreen/backButtonIdle.png",
            "LeaderboardScreen/backButtonActive.png"
        };
        
        enum m_textureNames {
            ModifiedLoginBackgroundTexture,
            LoginButtonIdleTexture,
            LoginButtonActiveTexture,
            BackButtonIdleTexture,
            BackButtonActiveTexture
        };

        enum m_spriteNames {
            ModifiedLoginBackground
        };

        enum m_buttonNames {
            LoginButton,
            BackButton
        };
};
