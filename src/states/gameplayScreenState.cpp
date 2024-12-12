#include "gameplayScreenState.hpp"
#include "menuScreenState.hpp"
#include "gameOverScreenState.hpp"
#include "../helpers/copy2dArray.hpp"
#include "../helpers/generateRandomBoard.hpp"
#include "../botLogic/bot.h"
#include "../helpers/convertArrayToVector.hpp"
#include <vector>
#include <iostream>
#include <unordered_map>

std::vector<sf::Sprite*> GameplayScreenState::sprites;
std::vector<Button*> GameplayScreenState::buttons;

GameplayScreenState::GameplayScreenState(StateManager& stateManager, sf::RenderWindow& window,
 int player1Grid[12][12], int difficulty) : State( stateManager, window ) {

    // Initialize ship locations
    // Player ship locations
    copy2dArray(player1Grid, m_playerShipLocations);

    // Enemy ship locations
    std::vector<std::vector<int>> enemyShipLocationsVector = generateRandomBoard();
    for (int i = 0; i <12; ++i) {
      for (int j = 0; j <12; ++j) {
        m_enemyShipLocations[i][j] = enemyShipLocationsVector[i][j];
        std::cout << enemyShipLocationsVector[i][j] << " ";
      }
      std::cout << std::endl;
    }

    // Get bot moves
    Bot* bot = new Bot();
    botMoves = bot->playGame(
        convertArrayToVector(m_playerShipLocations),
        difficulty
    );
    delete bot;
    bot = nullptr;

    // Initialize sprites if not already initialized
    if (GameplayScreenState::sprites.empty()) {
        GameplayScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::GameplayScreenBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (GameplayScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(0, 0),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::GiveUpButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::GiveUpButtonActiveTexture]),
            m_window
        ));
    }

    // Initialize Mini-Grid
    int startWidth = 20 * 4;
    int startHeight = 60 * 4;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            m_miniGrid[i][j] = new MiniGridCell(
                i, 
                j, 
                sf::Vector2f(startWidth + (j * 20), startHeight + (i * 20))
            );

            switch (m_playerShipLocations[i][j]) {
                case 1:
                    m_miniGrid[i][j]->setBottomTexture(
                        m_miniGridCellTextures::MiniShipBlueTexture
                    );
                    break;
                case 2:
                    m_miniGrid[i][j]->setBottomTexture(
                        m_miniGridCellTextures::MiniShipGreenTexture
                    );
                    break;
                case 3:
                    m_miniGrid[i][j]->setBottomTexture(
                        m_miniGridCellTextures::MiniShipPinkTexture
                    );
                    break;
                case 4:
                    m_miniGrid[i][j]->setBottomTexture(
                        m_miniGridCellTextures::MiniShipOrangeTexture
                    );
                    break;
                case 5:
                    m_miniGrid[i][j]->setBottomTexture(
                        m_miniGridCellTextures::MiniShipYellowTexture
                    );
                    break;
                default:
                    break;
            }

        }
    }

    // Initialize grid boundary
    m_gridBoundary = new sf::FloatRect(
        sf::Vector2f(100*4, 30*4),
        sf::Vector2f(120*4, 120*4)
    );

    // Initialize Grid
    startWidth = 100 * 4;
    startHeight = 30 * 4;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            m_grid[i][j] = new GridCell(
                i, 
                j, 
                sf::Vector2f(startWidth + (j * 40), startHeight + (i * 40)),
                true
            );
        }
    }

    // Initialize Ships
    // Key is shipIndicatorNumber
    // Value is a pair, where the first value is the position where the ship should be
    // and the second value is a bool if the ship should be rotated
    std::unordered_map<int, std::pair<sf::Vector2f, bool>> shipPositions;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            // If blue ship found
            if (shipPositions.find(1) == shipPositions.end()
             && m_enemyShipLocations[i][j] == 1) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocations[i][j + 1] == 1) {
                    // Horizontal
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40)) + 40
                    );
                    entry.second = true;
                } else {
                    // Vertical
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40))
                    );
                    entry.second = false;
                }
                shipPositions[1] = entry;
            }

            // If green ship found
            if (shipPositions.find(2) == shipPositions.end()
             && m_enemyShipLocations[i][j] == 2) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocations[i][j + 1] == 2) {
                    // Horizontal
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40)) + 40
                    );
                    entry.second = true;
                } else {
                    // Vertical
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40))
                    );
                    entry.second = false;
                }
                shipPositions[2] = entry;
            }

            // If pink ship found
            if (shipPositions.find(3) == shipPositions.end()
             && m_enemyShipLocations[i][j] == 3) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocations[i][j + 1] == 3) {
                    // Horizontal
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40)) + 40
                    );
                    entry.second = true;
                } else {
                    // Vertical
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40))
                    );
                    entry.second = false;
                }
                shipPositions[3] = entry;
            }

            // If orange ship found
            if (shipPositions.find(4) == shipPositions.end()
             && m_enemyShipLocations[i][j] == 4) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocations[i][j + 1] == 4) {
                    // Horizontal
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40)) + 40
                    );
                    entry.second = true;
                } else {
                    // Vertical
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40))
                    );
                    entry.second = false;
                }
                shipPositions[4] = entry;
            }

            // If yellow ship found
            if (shipPositions.find(5) == shipPositions.end()
             && m_enemyShipLocations[i][j] == 5) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocations[i][j + 1] == 5) {
                    // Horizontal
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40)) + 40
                    );
                    entry.second = true;
                } else {
                    // Vertical
                    entry.first = sf::Vector2f(
                        startWidth + (j * 40) + 4, (startHeight + (i * 40))
                    );
                    entry.second = false;
                }
                shipPositions[5] = entry;
            }
        }
    }

    m_blueShip = initializeSprite(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::BlueShipTexture]),
        shipPositions[1].first,
        sf::Vector2f(4,4)
    );
    if (shipPositions[1].second) m_blueShip->setRotation(-90);

    m_greenShip = initializeSprite(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::GreenShipTexture]),
        shipPositions[2].first,
        sf::Vector2f(4,4)
    );
    if (shipPositions[2].second) m_greenShip->setRotation(-90);

    m_pinkShip = initializeSprite(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::PinkShipTexture]),
        shipPositions[3].first,
        sf::Vector2f(4,4)
    );
    if (shipPositions[3].second) m_pinkShip->setRotation(-90);

    m_orangeShip = initializeSprite(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::OrangeShipTexture]),
        shipPositions[4].first,
        sf::Vector2f(4,4)
    );
    if (shipPositions[4].second) m_orangeShip->setRotation(-90);

    m_yellowShip = initializeSprite(
        *ResourceManager::getTexture(m_texturePaths[m_textureNames::YellowShipTexture]),
        shipPositions[5].first,
        sf::Vector2f(4,4)
    );
    if (shipPositions[5].second) m_yellowShip->setRotation(-90);


    // Initialize Ship X's
    for (int i = 0; i < 2; i++) {
        m_blueShipXs[i] = initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::ShipCubeHitTexture]),
            sf::Vector2f(250 * 4, (40 + i * 10) * 4),
            sf::Vector2f(4,4)
        );
    }
    for (int i = 0; i < 3; i++) {
        m_greenShipXs[i] = initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::ShipCubeHitTexture]),
            sf::Vector2f(250 * 4, (70 + i * 10) * 4),
            sf::Vector2f(4,4)
        );
    }
    for (int i = 0; i < 3; i++) {
        m_pinkShipXs[i] = initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::ShipCubeHitTexture]),
            sf::Vector2f(250 * 4, (110 + i * 10) * 4),
            sf::Vector2f(4,4)
        );
    }
    for (int i = 0; i < 4; i++) {
        m_orangeShipXs[i] = initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::ShipCubeHitTexture]),
            sf::Vector2f(280 * 4, (40 + i * 10) * 4),
            sf::Vector2f(4,4)
        );
    }
    for (int i = 0; i < 5; i++) {
        m_yellowShipXs[i] = initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::ShipCubeHitTexture]),
            sf::Vector2f(280 * 4, (90 + i * 10) * 4),
            sf::Vector2f(4,4)
        );
    }

    std::cout << "GameplayScreenState Initialized\n";
}

GameplayScreenState::~GameplayScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);

    // Delete MiniGridCells
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            delete m_miniGrid[i][j];
            m_miniGrid[i][j] = nullptr;
        }
    }

    // Delete GridCells
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            delete m_grid[i][j];
            m_grid[i][j] = nullptr;
        }
    }

    // Delete grid boundary
    delete m_gridBoundary;
    m_gridBoundary = nullptr;

    // Delete ships
    delete m_blueShip;
    m_blueShip = nullptr;
    delete m_greenShip;
    m_greenShip = nullptr;
    delete m_pinkShip;
    m_pinkShip = nullptr;
    delete m_orangeShip;
    m_orangeShip = nullptr;
    delete m_yellowShip;
    m_yellowShip = nullptr;

    // Delete ship X's
    for (sf::Sprite* sprite : m_blueShipXs) {
        delete sprite;
        sprite = nullptr;
    }
    for (sf::Sprite* sprite : m_greenShipXs) {
        delete sprite;
        sprite = nullptr;
    }
    for (sf::Sprite* sprite : m_pinkShipXs) {
        delete sprite;
        sprite = nullptr;
    }
    for (sf::Sprite* sprite : m_orangeShipXs) {
        delete sprite;
        sprite = nullptr;
    }
    for (sf::Sprite* sprite : m_yellowShipXs) {
        delete sprite;
        sprite = nullptr;
    }

    std::cout << "GameplayScreenState Destroyed\n";
}

bool GameplayScreenState::isShipSunk(int shipNumberIndicator, int row, int col) {
    // Check to the left
    int tempIndex = col;
    while (tempIndex > 0) {
        tempIndex--;
        if (m_enemyShipLocations[row][tempIndex] == shipNumberIndicator) 
            return false;
    }

    // Check to the right
    tempIndex = col;
    while (tempIndex < 11) {
        tempIndex++;
        if (m_enemyShipLocations[row][tempIndex] == shipNumberIndicator) 
            return false;
    }

    // Check up
    tempIndex = row;
    while (tempIndex > 0) {
        tempIndex--;
        if (m_enemyShipLocations[tempIndex][col] == shipNumberIndicator) 
            return false;
    }

    // Check down
    tempIndex = row;
    while (tempIndex < 11) {
        tempIndex++;
        if (m_enemyShipLocations[tempIndex][col] == shipNumberIndicator) 
            return false;
    }

    switch (shipNumberIndicator) {
        case 1:
            m_blueSank = true;
            break;
        case 2:
            m_greenSank = true;
            break;
        case 3:
            m_pinkSank = true;
            break;
        case 4:
            m_orangeSank = true;
            break;
        case 5:
            m_yellowSank = true;
            break;
        default:
            break;
    }
    return true;
}

bool GameplayScreenState::botTurn() {
    int row = botMoves[botMoveIndex].first;
    int col = botMoves[botMoveIndex].second;

    if (m_playerShipLocations[row][col] == 0) {
        // If miss
        m_miniGrid[row][col]->setTopTexture(
            m_miniGridCellTextures::MiniMissTexture
        );
    } else {
        // If hit
        m_miniGrid[row][col]->setTopTexture(
            m_miniGridCellTextures::MiniHitTexture
        );
    }

    botMoveIndex++;

    // If index reaches the end of the bot's moves, the bot wins
    return botMoveIndex == botMoves.size();
    
}

void GameplayScreenState::shipHitAttempt() {
    sf::Vector2f mousePosition = getMousePosition();
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (m_grid[i][j]->isInsideRect(mousePosition) && m_gridHitMap[i][j] == 0) {

                if (m_enemyShipLocations[i][j] == 0) {
                    // If miss
                    m_grid[i][j]->setSpriteTexture(
                        m_gridCellTextures::MissTexture
                    );

                    playSound("shipMiss.wav");
                } else {
                    // If hit
                    m_grid[i][j]->setSpriteTexture(
                        m_gridCellTextures::HitTexture
                    );

                    // Check if ship sank
                    int currentShipIndicator = m_enemyShipLocations[i][j];
                    m_enemyShipLocations[i][j] = 0;  // Set enemy ship location back to 0

                    if (isShipSunk(currentShipIndicator, i, j)) {
                        // Ship sank
                        playSound("shipSink.wav");

                        // Check for win
                        if (m_blueSank && m_greenSank && m_pinkSank 
                         && m_orangeSank && m_yellowSank) {
                            
                            // Update stats
                            if (!m_stateManager.m_user->getIsGuest()) {
                                m_stateManager.m_user->updateStats(
                                    m_stateManager.m_user->getTotalWins() + 1,
                                    m_stateManager.m_user->getTotalLosses(),
                                    m_stateManager.m_user->getTotalGames() + 1,
                                    (m_stateManager.m_user->getTotalWins() + 1.0) / (m_stateManager.m_user->getTotalGames() + 1.0)
                                );
                            }

                            std::unique_ptr<State> gameOverScreenState(
                                new GameOverScreenState(
                                    m_stateManager, m_window, 
                                    m_gameOverTextures::YouWin
                                )
                            );
                            m_stateManager.changeState(
                                std::move(gameOverScreenState)
                            );
                            return;
                        }
                    } else {
                        // Ship still alive
                        playSound("shipHit.wav");
                    }
                }
                
                m_gridHitMap[i][j] = 1;

                // Run bot's turn
                if (botTurn()) {
                    // If bot wins

                    // Update user stats
                    if (!m_stateManager.m_user->getIsGuest()) {
                        m_stateManager.m_user->updateStats(
                            m_stateManager.m_user->getTotalWins(),
                            m_stateManager.m_user->getTotalLosses() + 1,
                            m_stateManager.m_user->getTotalGames() + 1,
                            m_stateManager.m_user->getTotalWins() / (m_stateManager.m_user->getTotalGames() + 1.0)
                        );
                    }

                    std::unique_ptr<State> gameOverScreenState(new GameOverScreenState(
                        m_stateManager, m_window, m_gameOverTextures::YouLose
                    ));
                    m_stateManager.changeState(std::move(gameOverScreenState));
                    return;
                }
            }
        }
    }
}

void GameplayScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {

                // If left click on grid cell
                if (event.mouseButton.button == sf::Mouse::Left && isMouseInGrid) {
                    shipHitAttempt();
                    return;
                }
                
                // If left click on Give Up button
                if (event.mouseButton.button == sf::Mouse::Left
                 && GameplayScreenState::buttons[m_buttonNames::GiveUpButton]->getButtonState()) {
                    playSound("logoutButtonSelect.wav");
                    std::unique_ptr<State> menuScreenState(new MenuScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(menuScreenState));
                    return;
                }
                return;
            }
            default:
                break;
        }
    }
}

void GameplayScreenState::update() {
    sf::Vector2f mousePosition = GameplayScreenState::getMousePosition();
    buttons[m_buttonNames::GiveUpButton]->updateButtonState(mousePosition);

    isMouseInGrid = m_gridBoundary->contains(mousePosition) ? true : false;

    // Update grid hit markers
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            m_grid[i][j]->updateHitMarker(mousePosition);
        }
    }
    
}

void GameplayScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : GameplayScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : GameplayScreenState::buttons) {
        button->render();
    }

    // Render MiniGridCells
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
             m_miniGrid[i][j]->render(m_window);
        }
    }

    // Render Ships & Ship X's
    if (m_blueSank) {
        m_window.draw(*m_blueShip);
        for (sf::Sprite* sprite : m_blueShipXs) {
            m_window.draw(*sprite);
        }
    }
    if (m_greenSank) {
        m_window.draw(*m_greenShip);
        for (sf::Sprite* sprite : m_greenShipXs) {
            m_window.draw(*sprite);
        }
    }
    if (m_pinkSank) {
        m_window.draw(*m_pinkShip);
        for (sf::Sprite* sprite : m_pinkShipXs) {
            m_window.draw(*sprite);
        }
    }
    if (m_orangeSank) {
        m_window.draw(*m_orangeShip);
        for (sf::Sprite* sprite : m_orangeShipXs) {
            m_window.draw(*sprite);
        }
    }
    if (m_yellowSank) {
        m_window.draw(*m_yellowShip);
        for (sf::Sprite* sprite : m_yellowShipXs) {
            m_window.draw(*sprite);
        }
    }

    // Render GridCells
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
             m_grid[i][j]->render(m_window);
        }
    }

    m_window.display();
}
