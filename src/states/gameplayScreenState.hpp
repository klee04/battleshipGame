#pragma once
#include "../controllers/stateManager.hpp"
#include "../entity/gridCell.hpp"
#include "../entity/miniGridCell.hpp"
#include <utility>

class GameplayScreenState : public State {
    public:
        GameplayScreenState(StateManager& stateManager, sf::RenderWindow& window,
                            int player1Grid[12][12], int difficulty = 0);
        ~GameplayScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;

        void shipHitAttempt();

        // Grid Members
        GridCell* m_grid[12][12];
        int m_enemyShipLocations[12][12] = {};  // initialize as 2d array of zeros
        int m_gridHitMap[12][12] = {};  // 1 for shot attempt, 0 for no shot attempt
        sf::FloatRect* m_gridBoundary;
        bool isMouseInGrid = false;
        enum m_gridCellTextures {
            HitTexture,
            MissTexture
        };
        bool isShipSunk(int shipNumberIndicator, int row, int col);

        // Mini-Grid Members
        MiniGridCell* m_miniGrid[12][12];
        int m_playerShipLocations[12][12] = {};
        int m_miniGridHitMap[12][12] = {};  // 1 for shot attempt, 0 for no shot attempt
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
        bool m_blueSank = false;
        bool m_greenSank = false;
        bool m_pinkSank = false;
        bool m_orangeSank = false;
        bool m_yellowSank = false;
        sf::Sprite* m_blueShip;
        sf::Sprite* m_greenShip;
        sf::Sprite* m_pinkShip;
        sf::Sprite* m_orangeShip;
        sf::Sprite* m_yellowShip;

        // Bot Members
        std::vector<std::pair<int,int>> botMoves;
        int botMoveIndex = 0;
        bool botTurn();

        // Game Result Enum
        enum m_gameOverTextures {
            YouWin,
            YouLose,
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
            "FleetDeploymentScreen/ship5.png"
            
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
            YellowShipTexture
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
            GiveUpButton
        };
};