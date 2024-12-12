#pragma once
#include "../controllers/stateManager.hpp"
#include "../entity/userStatBar.hpp"
#include <unordered_map>

class LeaderboardScreenState : public State {
    public:
        LeaderboardScreenState(StateManager& stateManager, sf::RenderWindow& window);
        ~LeaderboardScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;
        std::vector<UserStatBar*> m_userStatBars;
        int m_currentStatIndex = 0;
        sf::Rect<float>* m_filterButtonRect;
        bool m_filterButtonHovered = false;
        sf::Text* m_filterText;

        std::string m_filterTextOptions[2] = {
            "Total Wins",
            "Total Games"
        };

        void updateUserStatBars(int statIndex);

        std::vector<std::string> m_texturePaths{
            "LeaderboardScreen/leaderboardBackground.png",
            "LeaderboardScreen/backButtonIdle.png",
            "LeaderboardScreen/backButtonActive.png",
            "LeaderboardScreen/yourStatsButtonIdle.png",
            "LeaderboardScreen/yourStatsButtonActive.png"
        };
        
        enum m_textureNames {
            LeaderboardBackgroundTexture,
            BackButtonIdleTexture,
            BackButtonActiveTexture,
            YourStatsButtonIdleTexture,
            YourStatsButtonActiveTexture
        };

        enum m_spriteNames {
            LeaderboardBackground
        };

        enum m_buttonNames {
            BackButton,
            YourStatsButton
        };
};
