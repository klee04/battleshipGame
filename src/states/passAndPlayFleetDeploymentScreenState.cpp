#include "passAndPlayFleetDeploymentScreenState.hpp"
#include "menuScreenState.hpp"
#include "passAndPlayGameplayScreenState.hpp"
#include "passToPlayerScreenState.hpp"
#include "../helpers/copy2dArray.hpp"

#include <iostream>
#include <unordered_map>

std::vector<sf::Sprite*> PassAndPlayFleetDeploymentScreenState::sprites;
std::vector<Button*> PassAndPlayFleetDeploymentScreenState::buttons;

PassAndPlayFleetDeploymentScreenState::PassAndPlayFleetDeploymentScreenState(StateManager& stateManager, sf::RenderWindow& window,
 int player1ShipLocations[12][12], bool isPlayerTwo) : State( stateManager, window ), m_isDragging(false), 
 m_shipBeingDragged(-1), isPlayerTwo(isPlayerTwo) {

    copy2dArray(player1ShipLocations, m_player1ShipLocations);

    // Initialize sprites if not already initialized
    if (PassAndPlayFleetDeploymentScreenState::sprites.empty()) {
        PassAndPlayFleetDeploymentScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::FleetDeploymentBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (PassAndPlayFleetDeploymentScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(240*4, 130*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::StartButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::StartButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(0, 0),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::HomeButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::HomeButtonActiveTexture]),
            m_window
        ));
    }

    // Initialize ships 
    ships.push_back(new Ship(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::Ship2Texture]),
        sf::Vector2f(30*4, 40*4),
        sf::Vector2f(4,4)
    ));

    ships.push_back(new Ship(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::Ship3aTexture]),
        sf::Vector2f(30*4, 70*4),
        sf::Vector2f(4,4)
    ));

    ships.push_back(new Ship(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::Ship3bTexture]),
        sf::Vector2f(30*4, 110*4),
        sf::Vector2f(4,4)
    ));

    ships.push_back(new Ship(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::Ship4Texture]),
        sf::Vector2f(60*4, 40*4),
        sf::Vector2f(4,4)
    ));

    ships.push_back(new Ship(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::Ship5Texture]),
        sf::Vector2f(60*4, 90*4),
        sf::Vector2f(4,4)
    ));
    

    // Initialize 12x12 grid
    int startWidth = 120 * 4;
    int startHeight = 30 * 4;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            m_grid[i][j] = new GridCell(
                i, 
                j, 
                sf::Vector2f(startWidth + (j * 40), startHeight + (i * 40))
            );
        }
    }

    // Initialize instruction text
    m_instructionText = initializeText(
        "Use the mouse to drag and drop your ships onto the grid!",
        sf::Vector2f(10 * 4, 166 * 4),
        15 * 4,
        sf::Color::White
    );

    // Initialize player indicator text
    m_playerIndicatorText = initializeText(
        isPlayerTwo ? "Player 2" : "Player 1",
        sf::Vector2f(255*4, 73*4),
        24 * 4,
        sf::Color::White
    );

    std::cout << "PassAndPlayFleetDeploymentScreenState Initialized\n";
}

PassAndPlayFleetDeploymentScreenState::~PassAndPlayFleetDeploymentScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);

    // Delete GridCells
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            delete m_grid[i][j];
            m_grid[i][j] = nullptr;
        }
    }

    // Delete Ships
    for (Ship* ship : ships) {
        delete ship;
        ship = nullptr;
    }

    // Delete text
    delete m_instructionText;
    m_instructionText = nullptr;
    delete m_playerIndicatorText;
    m_playerIndicatorText = nullptr;

    std::cout << "PassAndPlayFleetDeploymentScreenState Destroyed\n";
}

bool PassAndPlayFleetDeploymentScreenState::checkValidShipLocation(const int& row, const int& column, 
 const int& shipName, const bool& isShipHorizontal) {
    int shipNumberIndicator = 0;
    int shipLength = 0;
    if (shipName == m_shipNames::Ship2) {
        shipNumberIndicator = 1;
        shipLength = 2;
    } else if (shipName == m_shipNames::Ship3a) {
        shipNumberIndicator = 2;
        shipLength = 3;
    } else if (shipName == m_shipNames::Ship3b) {
        shipNumberIndicator = 3;
        shipLength = 3;
    } else if (shipName == m_shipNames::Ship4) {
        shipNumberIndicator = 4;
        shipLength = 4;
    } else if (shipName == m_shipNames::Ship5) {
        shipNumberIndicator = 5;
        shipLength = 5;
    }
    if (shipNumberIndicator == 0 || shipLength == 0) return false;

    // Check boundaries
    if (isShipHorizontal && column > 12 - shipLength) return false;
    if (!isShipHorizontal && row > 12 - shipLength) return false;

    // Check collisions with other ships
    int cellsLeftToCheck = shipLength;
    int rowIndex = row;
    int columnIndex = column;
    while (cellsLeftToCheck > 0) {
        if (m_shipLocations[rowIndex][columnIndex] != 0
         && m_shipLocations[rowIndex][columnIndex] != shipNumberIndicator) 
            return false;

        if (isShipHorizontal) 
            columnIndex++;
         else {
            rowIndex++;
        }

        cellsLeftToCheck--;
    }

    return true;
}

void PassAndPlayFleetDeploymentScreenState::fillShipLocation(const int& row, const int& column, 
 const int& shipName, const bool& isShipHorizontal) {
    int shipNumberIndicator = 0;
    int shipLength = 0;
    if (shipName == m_shipNames::Ship2) {
        shipNumberIndicator = 1;
        shipLength = 2;
    } else if (shipName == m_shipNames::Ship3a) {
        shipNumberIndicator = 2;
        shipLength = 3;
    } else if (shipName == m_shipNames::Ship3b) {
        shipNumberIndicator = 3;
        shipLength = 3;
    } else if (shipName == m_shipNames::Ship4) {
        shipNumberIndicator = 4;
        shipLength = 4;
    } else if (shipName == m_shipNames::Ship5) {
        shipNumberIndicator = 5;
        shipLength = 5;
    }
    if (shipNumberIndicator == 0 || shipLength == 0) return;

    // Set original location to zeros
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (m_shipLocations[i][j] == shipNumberIndicator) 
                m_shipLocations[i][j] = 0;
        }
    }

    // Fill in current location
    int cellsLeftToFill = shipLength;
    int rowIndex = row;
    int columnIndex = column;
    while (cellsLeftToFill > 0) {
        m_shipLocations[rowIndex][columnIndex] = shipNumberIndicator;

        if (isShipHorizontal) 
            columnIndex++;
        else {
            rowIndex++;
        }

        cellsLeftToFill--;
    }

}

void PassAndPlayFleetDeploymentScreenState::checkAllShipsPlaced() {
    bool result = true;
    std::unordered_map<int, bool> shipsPlacedMap = {
        {m_shipNames::Ship2, false},  
        {m_shipNames::Ship3a, false},  
        {m_shipNames::Ship3b, false},  
        {m_shipNames::Ship4, false}, 
        {m_shipNames::Ship5, false}, 
    };

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (m_shipLocations[i][j] == 1) 
                shipsPlacedMap[m_shipNames::Ship2] = true;
            else if (m_shipLocations[i][j] == 2) 
                shipsPlacedMap[m_shipNames::Ship3a] = true;
            else if (m_shipLocations[i][j] == 3) 
                shipsPlacedMap[m_shipNames::Ship3b] = true;
            else if (m_shipLocations[i][j] == 4) 
                shipsPlacedMap[m_shipNames::Ship4] = true; 
            else if (m_shipLocations[i][j] == 5) 
                shipsPlacedMap[m_shipNames::Ship5] = true;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (!shipsPlacedMap[i]) result = false;
    }

    m_allShipsPlaced = result;
}

void PassAndPlayFleetDeploymentScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonPressed: {

                if (event.mouseButton.button == sf::Mouse::Left
                 && PassAndPlayFleetDeploymentScreenState::ships[m_shipNames::Ship2]->getHoverState()) {
                    m_isDragging = true;
                    m_shipBeingDragged = m_shipNames::Ship2;
                    m_instructionText->setString("Press the spacebar to rotate the ship!");
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left
                 && PassAndPlayFleetDeploymentScreenState::ships[m_shipNames::Ship3a]->getHoverState()) {
                    m_isDragging = true;
                    m_shipBeingDragged = m_shipNames::Ship3a;
                    m_instructionText->setString("Press the spacebar to rotate the ship!");
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left
                 && PassAndPlayFleetDeploymentScreenState::ships[m_shipNames::Ship3b]->getHoverState()) {
                    m_isDragging = true;
                    m_shipBeingDragged = m_shipNames::Ship3b;
                    m_instructionText->setString("Press the spacebar to rotate the ship!");
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left
                 && PassAndPlayFleetDeploymentScreenState::ships[m_shipNames::Ship4]->getHoverState()) {
                    m_isDragging = true;
                    m_shipBeingDragged = m_shipNames::Ship4;
                    m_instructionText->setString("Press the spacebar to rotate the ship!");
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left
                 && PassAndPlayFleetDeploymentScreenState::ships[m_shipNames::Ship5]->getHoverState()) {
                    m_isDragging = true;
                    m_shipBeingDragged = m_shipNames::Ship5;
                    m_instructionText->setString("Press the spacebar to rotate the ship!");
                    return;
                }

                return;
            }
            case sf::Event::MouseButtonReleased: {

                // If release left click while dragging ship
                if (event.mouseButton.button == sf::Mouse::Left && m_isDragging) {

                    // Reset instruction text
                    m_instructionText->setString(
                        "Use the mouse to drag and drop your ships onto the grid!"
                    );
                    
                    // Loop through cells and check if cursor is within a cell
                    // If dragged to a valid position
                    sf::Vector2f mousePosition = PassAndPlayFleetDeploymentScreenState::getMousePosition();
                    for (int i = 0; i < 12; i++) {
                        for (int j = 0; j < 12; j++) {
                            if (m_grid[i][j]->isInsideRect(mousePosition)
                             && checkValidShipLocation(i, j, m_shipBeingDragged, 
                             ships[m_shipBeingDragged]->getIsHorizontal())) {

                                ships[m_shipBeingDragged]->setPosition(
                                    m_grid[i][j]->getPosition(),
                                    false
                                );

                                fillShipLocation(
                                    i, j, m_shipBeingDragged, 
                                    ships[m_shipBeingDragged]->getIsHorizontal()
                                );

                                m_isDragging = false;
                                m_shipBeingDragged = -1;

                                // Check if all ships placed
                                checkAllShipsPlaced();
                                
                                return;
                            }
                        }
                    }

                    // If not dragged to valid position
                    // Reset back to last valid position
                    ships[m_shipBeingDragged]->setPosition(
                        ships[m_shipBeingDragged]->getSavedPosition(),
                        false,
                        true
                    );
                    // Reset orientation 
                    if (ships[m_shipBeingDragged]->getIsHorizontal() != 
                     ships[m_shipBeingDragged]->getSavedIsHorizontal()) {
                        ships[m_shipBeingDragged]->rotate();
                    }
                    m_isDragging = false;
                    m_shipBeingDragged = -1;
                    return;
                }

                // If left click on home button
                if (event.mouseButton.button == sf::Mouse::Left
                 && PassAndPlayFleetDeploymentScreenState::buttons[m_buttonNames::HomeButton]->getButtonState()) {
                    playSound("buttonSelect.wav");

                    // Logout second player
                    delete m_stateManager.m_secondUser;
                    m_stateManager.m_secondUser = nullptr;

                    std::unique_ptr<State> menuScreenState(new MenuScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(menuScreenState));
                    return;
                }

                // If left click on start button
                if (event.mouseButton.button == sf::Mouse::Left
                 && PassAndPlayFleetDeploymentScreenState::buttons[m_buttonNames::StartButton]->getButtonState()
                 && m_allShipsPlaced) {
                    playSound("buttonSelect.wav");

                    if (isPlayerTwo) {
                        // Send to gameplay screen
                        int emptyArray[12][12] = {};
                        std::unique_ptr<State> passToPlayerScreenState(new PassToPlayerScreenState(
                            m_stateManager, m_window,
                            true, false,
                            emptyArray, false,
                            m_player1ShipLocations, m_shipLocations,
                            m_player1ShipLocations, m_shipLocations,
                            emptyArray, emptyArray,
                            emptyArray, emptyArray,
                            false, false, false, false, false,
                            false, false, false, false, false,
                            true
                        ));
                        m_stateManager.changeState(std::move(passToPlayerScreenState));
                        return;
                    } else {
                        // Send to player 2 fleet deployment
                        int emptyArray[12][12] = {};
                        std::unique_ptr<State> passToPlayerScreenState(new PassToPlayerScreenState(
                            m_stateManager, m_window,
                            false, true,
                            m_shipLocations, true,
                            emptyArray, emptyArray,
                            emptyArray, emptyArray,
                            emptyArray, emptyArray,
                            emptyArray, emptyArray,
                            false, false, false, false, false,
                            false, false, false, false, false,
                            true
                        ));
                        m_stateManager.changeState(std::move(passToPlayerScreenState));
                        return;
                    }
                }

                return;
            }
            case sf::Event::TextEntered: {
                // Spacebar
                if (event.text.unicode == 32 && m_isDragging) {
                    ships[m_shipBeingDragged]->rotate();
                    return;
                }

                return;
            }
            default:
                break;
        }
    }
}

void PassAndPlayFleetDeploymentScreenState::update() {
    sf::Vector2f mousePosition = PassAndPlayFleetDeploymentScreenState::getMousePosition();
    buttons[m_buttonNames::HomeButton]->updateButtonState(mousePosition);
    if (m_allShipsPlaced) buttons[m_buttonNames::StartButton]->updateButtonState(mousePosition);
    ships[m_shipNames::Ship2]->updateHoverState(mousePosition);
    ships[m_shipNames::Ship3a]->updateHoverState(mousePosition);
    ships[m_shipNames::Ship3b]->updateHoverState(mousePosition);
    ships[m_shipNames::Ship4]->updateHoverState(mousePosition);
    ships[m_shipNames::Ship5]->updateHoverState(mousePosition);

    if (m_isDragging) {
        switch (m_shipBeingDragged) {
            case m_shipNames::Ship2:
                ships[m_shipNames::Ship2]->setPosition(mousePosition, true);
                break;
            case m_shipNames::Ship3a:
                ships[m_shipNames::Ship3a]->setPosition(mousePosition, true);
                break;
            case m_shipNames::Ship3b:
                ships[m_shipNames::Ship3b]->setPosition(mousePosition, true);
                break;
            case m_shipNames::Ship4:
                ships[m_shipNames::Ship4]->setPosition(mousePosition, true);
                break;
            case m_shipNames::Ship5:
                ships[m_shipNames::Ship5]->setPosition(mousePosition, true);
                break;
            default:
                break;
        }
    }
}

void PassAndPlayFleetDeploymentScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : PassAndPlayFleetDeploymentScreenState::sprites) {
        m_window.draw(*sprite);
    }
    buttons[m_buttonNames::HomeButton]->render();
    if (m_allShipsPlaced) buttons[m_buttonNames::StartButton]->render();
    for (Ship* ship : PassAndPlayFleetDeploymentScreenState::ships) {
        ship->render(m_window);
    }

    m_window.draw(*m_instructionText);
    m_window.draw(*m_playerIndicatorText);

    m_window.display();
}
