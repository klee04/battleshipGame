#pragma once
#include "../controllers/stateManager.hpp"
#include "../entity/inputField.hpp"
#include "../entity/inputFieldBar.hpp"

class RegisterScreenState : public State {
    public:
        RegisterScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~RegisterScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        InputField m_usernameField;
        InputField m_passwordField;
        InputField m_confirmPasswordField;

        InputFieldBar m_inputFieldBar;

        sf::Text m_errorText;

        std::vector<std::string> m_texturePaths{
            "RegisterScreen/registerBackground.png",
            "RegisterScreen/registerButtonIdle.png",
            "RegisterScreen/registerButtonActive.png",
            "RegisterScreen/backButtonIdle.png",
            "RegisterScreen/backButtonActive.png",
        };
        
        enum m_textureNames {
            RegisterBackgroundTexture,
            RegisterButtonIdleTexture,
            RegisterButtonActiveTexture,
            BackButtonIdleTexture,
            BackButtonActiveTexture
        };

        enum m_spriteNames {
            RegisterBackground
        };

        enum m_buttonNames {
            RegisterButton,
            BackButton
        };
};