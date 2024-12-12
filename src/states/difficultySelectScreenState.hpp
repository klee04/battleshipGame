#pragma once
#include "../controllers/stateManager.hpp"

class DifficultySelectScreenState : public State {
    public:
        DifficultySelectScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~DifficultySelectScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        std::vector<std::string> m_texturePaths{
            "DifficultySelectScreen/difficultySelectBackground.png",
            "DifficultySelectScreen/easyButtonIdle.png",
            "DifficultySelectScreen/easyButtonActive.png",
            "DifficultySelectScreen/mediumButtonIdle.png",
            "DifficultySelectScreen/mediumButtonActive.png",
            "DifficultySelectScreen/hardButtonIdle.png",
            "DifficultySelectScreen/hardButtonActive.png",
            "DifficultySelectScreen/backButtonIdle.png",
            "DifficultySelectScreen/backButtonActive.png"
        };
        
        enum m_textureNames {
            DifficultySelectBackgroundTexture,
            EasyButtonIdleTexture,
            EasyButtonActiveTexture,
            MediumButtonIdleTexture,
            MediumButtonActiveTexture,
            HardButtonIdleTexture,
            HardButtonActiveTexture,
            BackButtonIdleTexture,
            BackButtonActiveTexture
        };

        enum m_spriteNames {
            DifficultySelectBackground
        };

        enum m_buttonNames {
            EasyButton,
            MediumButton,
            HardButton,
            BackButton
        };
};
