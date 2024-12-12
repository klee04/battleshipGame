#pragma once
#include "../controllers/stateManager.hpp"

class GameOverScreenState : public State {
    public:
        GameOverScreenState(StateManager& stateManager, sf::RenderWindow& window,
                            int gameResultIndex);
        ~GameOverScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        std::vector<std::string> m_texturePaths{
            "GameOverScreen/youWinBackground.png",
            "GameOverScreen/youLoseBackground.png",
            "GameOverScreen/player1WinsBackground.png",
            "GameOverScreen/player2WinsBackground.png",
            "GameOverScreen/menuButtonIdle.png",
            "GameOverScreen/menuButtonActive.png",
        };
        
        enum m_textureNames {
            YouWinBackgroundTexture,
            YouLoseBackgroundTexture,
            Player1WinsBackgroundTexture,
            Player2WinsBackgroundTexture,
            MenuButtonIdleTexture,
            MenuButtonActiveTexture
        };

        enum m_spriteNames {
            Background
        };

        enum m_buttonNames {
            MenuButton
        };
};