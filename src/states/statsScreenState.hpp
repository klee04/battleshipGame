#pragma once
#include "../controllers/stateManager.hpp"

class StatsScreenState : public State {
    public:
        StatsScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~StatsScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;
        sf::Text* m_totalWinsText;
        sf::Text* m_totalLossesText;
        sf::Text* m_totalGamesText;
        sf::Text* m_winPercentageText;

        std::vector<std::string> m_texturePaths{
            "StatsScreen/statsBackground.png",
            "StatsScreen/backButtonIdle.png",
            "StatsScreen/backButtonActive.png",
        };
        
        enum m_textureNames {
            StatsBackgroundTexture,
            BackButtonIdleTexture,
            BackButtonActiveTexture,
        };

        enum m_spriteNames {
            StatsBackground
        };

        enum m_buttonNames {
            BackButton,
        };
};
