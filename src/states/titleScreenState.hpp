#pragma once
#include "../controllers/stateManager.hpp"

class TitleScreenState : public State {
    public:
        TitleScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~TitleScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        bool animationSwitch;

        std::vector<std::string> m_texturePaths{
            "TitleScreen/titleScreen.png",
            "TitleScreen/playButtonIdle.png",
            "TitleScreen/playButtonActive.png",
            "TitleScreen/titleText.png"
        };
        
        enum m_textureNames {
            TitleScreenTexture,
            PlayButtonIdleTexture,
            PlayButtonActiveTexture,
            TitleTextTexture
        };

        enum m_spriteNames {
            TitleScreen,
            TitleText
        };

        enum m_buttonNames {
            PlayButton
        };
};
