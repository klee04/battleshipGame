#include "passAndPlayGameplayScreenState.hpp"
#include "menuScreenState.hpp"
#include "gameOverScreenState.hpp"
#include "passToPlayerScreenState.hpp"
#include "../helpers/copy2dArray.hpp"
#include "../helpers/convertArrayToVector.hpp"
#include <vector>
#include <iostream>
#include <unordered_map>

std::vector<sf::Sprite*> PassAndPlayGameplayScreenState::sprites;
std::vector<Button*> PassAndPlayGameplayScreenState::buttons;

PassAndPlayGameplayScreenState::PassAndPlayGameplayScreenState(StateManager& stateManager, sf::RenderWindow& window,
 int playerGridOriginal[12][12], int enemyGridOriginal[12][12], int playerGridCurrent[12][12], 
 int enemyGridCurrent[12][12], int playerHitAttemptMap[12][12], int enemyHitAttemptMap[12][12], 
 int playerHitMissMap[12][12], int enemyHitMissMap[12][12],
 bool enemyBlueSank, bool enemyGreenSank, bool enemyPinkSank, 
 bool enemyOrangeSank, bool enemyYellowSank, bool playerBlueSank, 
 bool playerGreenSank, bool playerPinkSank, bool playerOrangeSank, 
 bool playerYellowSank, bool isPlayerOne) 
 : State( stateManager, window ), isPlayerOne(isPlayerOne) {

    // Initialize ship locations
    // Player ship locations
    copy2dArray(playerGridOriginal, m_playerShipLocationsOriginal);
    copy2dArray(playerGridCurrent, m_playerShipLocationsCurrent);

    // Enemy ship locations
    copy2dArray(enemyGridOriginal, m_enemyShipLocationsOriginal);
    copy2dArray(enemyGridCurrent, m_enemyShipLocationsCurrent);

    // Hit Miss Maps
    copy2dArray(playerHitMissMap, m_miniGridHitMissMap);
    copy2dArray(enemyHitMissMap, m_gridHitMissMap);

    // Hit Attempt Maps
    copy2dArray(playerHitAttemptMap, m_miniGridHitAttemptMap);
    copy2dArray(enemyHitAttemptMap, m_gridHitAttemptMap);

    // Initialize ships sank
    m_enemyBlueSank = enemyBlueSank;
    m_enemyGreenSank = enemyGreenSank;
    m_enemyPinkSank = enemyPinkSank;
    m_enemyOrangeSank = enemyOrangeSank; 
    m_enemyYellowSank = enemyYellowSank;
    m_playerBlueSank = playerBlueSank;
    m_playerGreenSank = playerGreenSank;
    m_playerPinkSank = playerPinkSank;
    m_playerOrangeSank = playerOrangeSank; 
    m_playerYellowSank = playerYellowSank;

    // Initialize player indicator text
    m_playerIndicatorText = initializeText(
        isPlayerOne ? "Player 1's Turn" : "Player 2's Turn",
        sf::Vector2f(115*4, 163*4),
        24*4,
        sf::Color::White
    );

    // Initialize sprites if not already initialized
    if (PassAndPlayGameplayScreenState::sprites.empty()) {
        PassAndPlayGameplayScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::GameplayScreenBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (PassAndPlayGameplayScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(0, 0),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::GiveUpButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::GiveUpButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(300*4, 160*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::RightButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::RightButtonActiveTexture]),
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

            switch (m_playerShipLocationsOriginal[i][j]) {
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
             && m_enemyShipLocationsOriginal[i][j] == 1) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocationsOriginal[i][j + 1] == 1) {
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
             && m_enemyShipLocationsOriginal[i][j] == 2) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocationsOriginal[i][j + 1] == 2) {
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
             && m_enemyShipLocationsOriginal[i][j] == 3) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocationsOriginal[i][j + 1] == 3) {
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
             && m_enemyShipLocationsOriginal[i][j] == 4) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocationsOriginal[i][j + 1] == 4) {
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
             && m_enemyShipLocationsOriginal[i][j] == 5) {
                std::pair<sf::Vector2f, bool> entry;
                if (j < 11 && m_enemyShipLocationsOriginal[i][j + 1] == 5) {
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

    // Initialize grid textures
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (m_gridHitMissMap[i][j] == 1) {
                m_grid[i][j]->setSpriteTexture(
                    m_gridCellTextures::HitTexture
                );
            } else if (m_gridHitMissMap[i][j] == -1) {
                m_grid[i][j]->setSpriteTexture(
                    m_gridCellTextures::MissTexture
                );
            }
        }
    }

    // Initialize mini grid textures
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (m_miniGridHitMissMap[i][j] == 1) {
                m_miniGrid[i][j]->setTopTexture(
                    m_miniGridCellTextures::MiniHitTexture
                );
            } else if (m_miniGridHitMissMap[i][j] == -1) {
                m_miniGrid[i][j]->setTopTexture(
                    m_miniGridCellTextures::MiniMissTexture
                );
            }
        }
    }


    std::cout << "PassAndPlayGameplayScreenState Initialized\n";
}

PassAndPlayGameplayScreenState::~PassAndPlayGameplayScreenState() {
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

    // Delete text
    delete m_playerIndicatorText;
    m_playerIndicatorText = nullptr;

    std::cout << "PassAndPlayGameplayScreenState Destroyed\n";
}

bool PassAndPlayGameplayScreenState::isShipSunk(int shipNumberIndicator, int row, int col) {
    // Check to the left
    int tempIndex = col;
    while (tempIndex > 0) {
        tempIndex--;
        if (m_enemyShipLocationsCurrent[row][tempIndex] == shipNumberIndicator) 
            return false;
    }

    // Check to the right
    tempIndex = col;
    while (tempIndex < 11) {
        tempIndex++;
        if (m_enemyShipLocationsCurrent[row][tempIndex] == shipNumberIndicator) 
            return false;
    }

    // Check up
    tempIndex = row;
    while (tempIndex > 0) {
        tempIndex--;
        if (m_enemyShipLocationsCurrent[tempIndex][col] == shipNumberIndicator) 
            return false;
    }

    // Check down
    tempIndex = row;
    while (tempIndex < 11) {
        tempIndex++;
        if (m_enemyShipLocationsCurrent[tempIndex][col] == shipNumberIndicator) 
            return false;
    }

    switch (shipNumberIndicator) {
        case 1:
            m_enemyBlueSank = true;
            break;
        case 2:
            m_enemyGreenSank = true;
            break;
        case 3:
            m_enemyPinkSank = true;
            break;
        case 4:
            m_enemyOrangeSank = true;
            break;
        case 5:
            m_enemyYellowSank = true;
            break;
        default:
            break;
    }
    return true;
}

void PassAndPlayGameplayScreenState::shipHitAttempt() {
    sf::Vector2f mousePosition = getMousePosition();
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (m_grid[i][j]->isInsideRect(mousePosition) && m_gridHitAttemptMap[i][j] == 0) {

                if (m_enemyShipLocationsCurrent[i][j] == 0) {
                    // If miss
                    m_grid[i][j]->setSpriteTexture(
                        m_gridCellTextures::MissTexture
                    );

                    m_gridHitMissMap[i][j] = -1;

                    playSound("shipMiss.wav");
                } else {
                    // If hit
                    m_grid[i][j]->setSpriteTexture(
                        m_gridCellTextures::HitTexture
                    );

                    m_gridHitMissMap[i][j] = 1;

                    // Check if ship sank
                    int currentShipIndicator = m_enemyShipLocationsCurrent[i][j];
                    m_enemyShipLocationsCurrent[i][j] = 0;  // Set enemy ship location back to 0

                    if (isShipSunk(currentShipIndicator, i, j)) {
                        // Ship sank
                        playSound("shipSink.wav");

                        // Check for win
                        if (m_enemyBlueSank && m_enemyGreenSank && m_enemyPinkSank 
                         && m_enemyOrangeSank && m_enemyYellowSank) {
                            
                            // Update stats
                            if (isPlayerOne) {
                                // Player 1 Wins, Player 2 Losses
                                if (!m_stateManager.m_user->getIsGuest()) {
                                    m_stateManager.m_user->updateStats(
                                        m_stateManager.m_user->getTotalWins() + 1,
                                        m_stateManager.m_user->getTotalLosses(),
                                        m_stateManager.m_user->getTotalGames() + 1,
                                        (m_stateManager.m_user->getTotalWins() + 1.0) / (m_stateManager.m_user->getTotalGames() + 1.0)
                                    );
                                }

                                if (!m_stateManager.m_secondUser->getIsGuest()) {
                                    m_stateManager.m_secondUser->updateStats(
                                        m_stateManager.m_secondUser->getTotalWins(),
                                        m_stateManager.m_secondUser->getTotalLosses() + 1,
                                        m_stateManager.m_secondUser->getTotalGames() + 1,
                                        (m_stateManager.m_secondUser->getTotalWins()) / (m_stateManager.m_user->getTotalGames() + 1.0)
                                    );
                                }
                            } else {
                                // Player 1 Losses, Player 2 Wins
                                if (!m_stateManager.m_user->getIsGuest()) {
                                    m_stateManager.m_user->updateStats(
                                        m_stateManager.m_user->getTotalWins(),
                                        m_stateManager.m_user->getTotalLosses() + 1,
                                        m_stateManager.m_user->getTotalGames() + 1,
                                        (m_stateManager.m_user->getTotalWins()) / (m_stateManager.m_user->getTotalGames() + 1.0)
                                    );
                                }

                                if (!m_stateManager.m_secondUser->getIsGuest()) {
                                    m_stateManager.m_user->updateStats(
                                        m_stateManager.m_secondUser->getTotalWins() + 1,
                                        m_stateManager.m_secondUser->getTotalLosses(),
                                        m_stateManager.m_secondUser->getTotalGames() + 1,
                                        (m_stateManager.m_secondUser->getTotalWins() + 1.0) / (m_stateManager.m_user->getTotalGames() + 1.0)
                                    );
                                }
                            }

                            // Logout second player
                            delete m_stateManager.m_secondUser;
                            m_stateManager.m_secondUser = nullptr;

                            std::unique_ptr<State> gameOverScreenState(
                                new GameOverScreenState(
                                    m_stateManager, m_window, 
                                    isPlayerOne ? m_gameOverTextures::Player1Wins : m_gameOverTextures::Player2Wins
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
                
                m_gridHitAttemptMap[i][j] = 1;
                hitAttemptMade = true;

                return;
            }
        }
    }
}

void PassAndPlayGameplayScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {

                // If left click on grid cell
                if (event.mouseButton.button == sf::Mouse::Left && isMouseInGrid
                 && !hitAttemptMade) {
                    shipHitAttempt();
                    return;
                }
                
                // If left click on Give Up button
                if (event.mouseButton.button == sf::Mouse::Left
                 && PassAndPlayGameplayScreenState::buttons[m_buttonNames::GiveUpButton]->getButtonState()) {
                    playSound("logoutButtonSelect.wav");

                    // Logout Second Player
                    delete m_stateManager.m_secondUser;
                    m_stateManager.m_secondUser = nullptr;

                    std::unique_ptr<State> menuScreenState(new MenuScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(menuScreenState));
                    return;
                }

                // If left click on Right button
                if (event.mouseButton.button == sf::Mouse::Left
                 && PassAndPlayGameplayScreenState::buttons[m_buttonNames::RightButton]->getButtonState()
                 && hitAttemptMade) {
                    playSound("buttonSelect.wav");

                    // Go to enemy's turn
                    int emptyArray[12][12] = {};
                    std::unique_ptr<State> passToPlayerScreenState(new PassToPlayerScreenState(
                        m_stateManager, m_window,
                        isPlayerOne ? false : true, false,
                        emptyArray, false,
                        m_enemyShipLocationsOriginal, m_playerShipLocationsOriginal,
                        m_enemyShipLocationsCurrent, m_playerShipLocationsCurrent,
                        m_gridHitAttemptMap, m_miniGridHitAttemptMap,
                        m_gridHitMissMap, m_miniGridHitMissMap,
                        m_playerBlueSank, m_playerGreenSank, m_playerPinkSank,
                        m_playerOrangeSank, m_playerYellowSank, m_enemyBlueSank, 
                        m_enemyGreenSank, m_enemyPinkSank, m_enemyOrangeSank, m_enemyYellowSank,
                        isPlayerOne ? false : true
                    ));
                    m_stateManager.changeState(std::move(passToPlayerScreenState));
    
                    return;
                }
                return;
            }
            default:
                break;
        }
    }
}

void PassAndPlayGameplayScreenState::update() {
    sf::Vector2f mousePosition = PassAndPlayGameplayScreenState::getMousePosition();
    buttons[m_buttonNames::GiveUpButton]->updateButtonState(mousePosition);
    if (hitAttemptMade)
        buttons[m_buttonNames::RightButton]->updateButtonState(mousePosition);

    isMouseInGrid = m_gridBoundary->contains(mousePosition) ? true : false;

    // Update grid hit markers
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            m_grid[i][j]->updateHitMarker(mousePosition, hitAttemptMade);
        }
    }
    
}

void PassAndPlayGameplayScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : PassAndPlayGameplayScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : PassAndPlayGameplayScreenState::buttons) {
        if (button == buttons[m_buttonNames::RightButton] && !hitAttemptMade)
            continue;

        button->render();
    }

    // Render MiniGridCells
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
             m_miniGrid[i][j]->render(m_window);
        }
    }

    // Render Ships & Ship X's
    if (m_enemyBlueSank) {
        m_window.draw(*m_blueShip);
        for (sf::Sprite* sprite : m_blueShipXs) {
            m_window.draw(*sprite);
        }
    }
    if (m_enemyGreenSank) {
        m_window.draw(*m_greenShip);
        for (sf::Sprite* sprite : m_greenShipXs) {
            m_window.draw(*sprite);
        }
    }
    if (m_enemyPinkSank) {
        m_window.draw(*m_pinkShip);
        for (sf::Sprite* sprite : m_pinkShipXs) {
            m_window.draw(*sprite);
        }
    }
    if (m_enemyOrangeSank) {
        m_window.draw(*m_orangeShip);
        for (sf::Sprite* sprite : m_orangeShipXs) {
            m_window.draw(*sprite);
        }
    }
    if (m_enemyYellowSank) {
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

    m_window.draw(*m_playerIndicatorText);

    m_window.display();
}
