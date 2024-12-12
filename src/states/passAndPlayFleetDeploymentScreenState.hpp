#pragma once
#include "../controllers/stateManager.hpp"
#include "../entity/ship.hpp"
#include "../entity/gridCell.hpp"

class PassAndPlayFleetDeploymentScreenState : public State {
    public:
        PassAndPlayFleetDeploymentScreenState(StateManager& stateManager, sf::RenderWindow& window,
                                              int player1ShipLocations[12][12], bool isPlayerTwo);
        ~PassAndPlayFleetDeploymentScreenState();

        void processEvents() override;
        void update() override;
        void draw() override;

    private:
        static std::vector<sf::Sprite*> sprites;
        static std::vector<Button*> buttons;
        std::vector<Ship*> ships;
        sf::Text* m_instructionText;
        int m_botDifficulty;

        // TODO: add P1 & P2 indicator for Pass & Play mode
        bool isPlayerTwo;
        sf::Text* m_playerIndicatorText;

        // Grid Members
        GridCell* m_grid[12][12];
        bool m_isDragging;
        int m_shipBeingDragged;
        int m_shipLocations[12][12] = {};  // initialize as 2d array of zeros
        int m_player1ShipLocations[12][12] = {};
        bool checkValidShipLocation(const int& row, const int& column, 
                                    const int& shipName, const bool& isShipHorizontal);
        void fillShipLocation(const int& row, const int& column, 
                              const int& shipName, const bool& isShipHorizontal);
        void checkAllShipsPlaced();
        bool m_allShipsPlaced = false;

        std::vector<std::string> m_texturePaths{
            "FleetDeploymentScreen/fleetDeploymentBackground.png",
            "FleetDeploymentScreen/ship2.png",
            "FleetDeploymentScreen/ship3a.png",
            "FleetDeploymentScreen/ship3b.png",
            "FleetDeploymentScreen/ship4.png",
            "FleetDeploymentScreen/ship5.png",
            "FleetDeploymentScreen/startButtonIdle.png",
            "FleetDeploymentScreen/startButtonActive.png",
            "FleetDeploymentScreen/homeButtonIdle.png",
            "FleetDeploymentScreen/homeButtonActive.png"
        };
        
        enum m_textureNames {
            FleetDeploymentBackgroundTexture,
            Ship2Texture,
            Ship3aTexture,
            Ship3bTexture,
            Ship4Texture,
            Ship5Texture,
            StartButtonIdleTexture,
            StartButtonActiveTexture,
            HomeButtonIdleTexture,
            HomeButtonActiveTexture
        };

        enum m_spriteNames {
            FleetDeploymentBackground,
        };

        enum m_buttonNames {
            StartButton,
            HomeButton
        };

        enum m_shipNames {
            Ship2, 
            Ship3a,
            Ship3b,
            Ship4,
            Ship5
        };
};
