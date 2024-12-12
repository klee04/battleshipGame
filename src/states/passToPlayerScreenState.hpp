#pragma once
#include "../controllers/stateManager.hpp"

class PassToPlayerScreenState : public State {
    public:
        PassToPlayerScreenState(StateManager& stateManager, sf::RenderWindow& window,
                                const bool passToPlayerOne, const bool fleetDeployment,
                                int FD_player1ShipLocations[12][12], bool FD_isPlayerTwo,
                                int G_playerGridOriginal[12][12], int G_enemyGridOriginal[12][12],
                                int G_playerGridCurrent[12][12], int G_enemyGridCurrent[12][12],
                                int G_playerHitAttemptMap[12][12], int G_enemyHitAttemptMap[12][12],
                                int G_playerHitMissMap[12][12], int G_enemyHitMissMap[12][12],
                                bool G_enemyBlueSank, bool G_enemyGreenSank, 
                                bool G_enemyPinkSank, bool G_enemyOrangeSank, bool G_enemyYellowSank,
                                bool G_playerBlueSank, bool G_playerGreenSank, 
                                bool G_playerPinkSank, bool G_playerOrangeSank, bool G_playerYellowSank,
                                bool G_isPlayerOne);
        ~PassToPlayerScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        const bool fleetDeployment;
        int FD_player1ShipLocations[12][12];
        bool FD_isPlayerTwo;
        int G_playerGridOriginal[12][12];
        int G_enemyGridOriginal[12][12];
        int G_playerGridCurrent[12][12];
        int G_enemyGridCurrent[12][12];
        int G_playerHitAttemptMap[12][12];
        int G_enemyHitAttemptMap[12][12];
        int G_playerHitMissMap[12][12];
        int G_enemyHitMissMap[12][12];
        bool G_enemyBlueSank;
        bool G_enemyGreenSank;
        bool G_enemyPinkSank;
        bool G_enemyOrangeSank; 
        bool G_enemyYellowSank;
        bool G_playerBlueSank;
        bool G_playerGreenSank;
        bool G_playerPinkSank;
        bool G_playerOrangeSank;
        bool G_playerYellowSank;
        bool G_isPlayerOne;

        std::vector<std::string> m_texturePaths{
            "PassToPlayerScreen/passToP1Background.png",
            "PassToPlayerScreen/passToP2Background.png",
            "PassToPlayerScreen/continueButtonIdle.png",
            "PassToPlayerScreen/continueButtonActive.png"
        };
        
        enum m_textureNames {
            PassToP1BackgroundTexture,
            PassToP2BackgroundTexture,
            ContinueButtonIdleTexture,
            ContinueButtonActiveTexture
        };

        enum m_spriteNames {
            PassToPlayerBackground
        };

        enum m_buttonNames {
            ContinueButton
        };
};