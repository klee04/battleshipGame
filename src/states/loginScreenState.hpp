#pragma once
#include "../controllers/stateManager.hpp"
#include "../entity/inputFieldBar.hpp"
#include "../entity/inputField.hpp"

class LoginScreenState : public State {
    public:
        LoginScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~LoginScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        InputField m_usernameField;
        InputField m_passwordField;

        InputFieldBar m_inputFieldBar;

        sf::Text m_errorText;

        std::vector<std::string> m_texturePaths{
            "LoginScreen/loginBackground.png",
            "LoginScreen/loginButtonIdle.png",
            "LoginScreen/loginButtonActive.png",
            "LoginScreen/registerButtonIdle.png",
            "LoginScreen/registerButtonActive.png",
            "LoginScreen/guestButtonIdle.png",
            "LoginScreen/guestButtonActive.png"
        };
        
        enum m_textureNames {
            LoginBackgroundTexture,
            LoginButtonIdleTexture,
            LoginButtonActiveTexture,
            RegisterButtonIdleTexture,
            RegisterButtonActiveTexture,
            GuestButtonIdleTexture,
            GuestButtonActiveTexture
        };

        enum m_spriteNames {
            LoginBackground
        };

        enum m_buttonNames {
            LoginButton,
            RegisterButton,
            GuestButton
        };
};
