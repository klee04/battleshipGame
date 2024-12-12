#pragma once
#include "../controllers/stateManager.hpp"
#include "../entity/gridCell.hpp"
#include "../entity/miniGridCell.hpp"
#include <utility>

class PassAndPlayGameplayScreenState : public State {
    public:
        PassAndPlayGameplayScreenState(StateManager& stateManager, sf::RenderWindow& window,
                                       int playerGridOriginal[12][12], int enemyGridOriginal[12][12],
                                       int playerGridCurrent[12][12], int enemyGridCurrent[12][12],
                                       int playerHitAttemptMap[12][12], int enemyHitAttemptMap[12][12],
                                       int playerHitMissMap[12][12], int enemyHitMissMap[12][12],
                                       bool enemyBlueSank, bool enemyGreenSank, 
                                       bool enemyPinkSank, bool enemyOrangeSank, bool enemyYellowSank,
                                       bool playerBlueSank, bool playerGreenSank, 
                                       bool playerPinkSank, bool playerOrangeSank, bool playerYellowSank,
                                       bool isPlayerOne);
        ~PassAndPlayGameplayScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        bool isPlayerOne;
        bool hitAttemptMade = false;
        void shipHitAttempt();

        sf::Text* m_playerIndicatorText;

        // Grid Members
        GridCell* m_grid[12][12];
        int m_enemyShipLocationsOriginal[12][12] = {};  // initialize as 2d array of zeros
        int m_enemyShipLocationsCurrent[12][12] = {};
        int m_gridHitAttemptMap[12][12] = {};  // 1 for shot attempt, 0 for no shot attempt
        int m_gridHitMissMap[12][12] = {};  // 0 for nothing, 1 for hit, -1 for miss
        sf::FloatRect* m_gridBoundary;
        bool isMouseInGrid = false;
        enum m_gridCellTextures {
            HitTexture,
            MissTexture
        };
        bool isShipSunk(int shipNumberIndicator, int row, int col);

        // Mini-Grid Members
        MiniGridCell* m_miniGrid[12][12];
        int m_playerShipLocationsOriginal[12][12] = {};
        int m_playerShipLocationsCurrent[12][12] = {};
        int m_miniGridHitAttemptMap[12][12] = {};  // 1 for shot attempt, 0 for no shot attempt
        int m_miniGridHitMissMap[12][12] = {};  // 0 for nothing, 1 for hit, -1 for miss
        enum m_miniGridCellTextures {
            MiniHitTexture,
            MiniMissTexture,
            MiniShipBlueTexture,
            MiniShipGreenTexture,
            MiniShipPinkTexture,
            MiniShipOrangeTexture,
            MiniShipYellowTexture
        };

        // Ship Members
        sf::Sprite* m_blueShipXs[2];
        sf::Sprite* m_greenShipXs[3];
        sf::Sprite* m_pinkShipXs[3];
        sf::Sprite* m_orangeShipXs[4];
        sf::Sprite* m_yellowShipXs[5];
        bool m_enemyBlueSank;
        bool m_enemyGreenSank;
        bool m_enemyPinkSank;
        bool m_enemyOrangeSank;
        bool m_enemyYellowSank;
        sf::Sprite* m_blueShip;
        sf::Sprite* m_greenShip;
        sf::Sprite* m_pinkShip;
        sf::Sprite* m_orangeShip;
        sf::Sprite* m_yellowShip;
        bool m_playerBlueSank;
        bool m_playerGreenSank;
        bool m_playerPinkSank;
        bool m_playerOrangeSank;
        bool m_playerYellowSank;

        // Game Result Enum
        enum m_gameOverTextures {
            YouWin,
            YouLose,
            Player1Wins,
            Player2Wins
        };

        std::vector<std::string> m_texturePaths{
            "GameplayScreen/gameplayScreenBackground.png",
            "GameplayScreen/giveUpButtonIdle.png",
            "GameplayScreen/giveUpButtonActive.png",
            "GameplayScreen/shipCubeHit.png",
            "FleetDeploymentScreen/ship2.png",
            "FleetDeploymentScreen/ship3a.png",
            "FleetDeploymentScreen/ship3b.png",
            "FleetDeploymentScreen/ship4.png",
            "FleetDeploymentScreen/ship5.png",
            "UserSettingsScreen/rightButtonIdle.png",
            "UserSettingsScreen/rightButtonActive.png",
        };
        
        enum m_textureNames {
            GameplayScreenBackgroundTexture,
            GiveUpButtonIdleTexture,
            GiveUpButtonActiveTexture,
            ShipCubeHitTexture,
            BlueShipTexture,
            GreenShipTexture,
            PinkShipTexture,
            OrangeShipTexture,
            YellowShipTexture,
            RightButtonIdleTexture,
            RightButtonActiveTexture
        };

        enum m_spriteNames {
            GameplayScreenBackground,
            ShipCubeHit,
            BlueShip,
            GreenShip,
            PinkShip,
            OrangeShip,
            YellowShip
        };

        enum m_buttonNames {
            GiveUpButton,
            RightButton
        };
};