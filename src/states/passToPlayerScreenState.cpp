#include "passToPlayerScreenState.hpp"
#include "passAndPlayFleetDeploymentScreenState.hpp"
#include "passAndPlayGameplayScreenState.hpp"
#include "../helpers/copy2dArray.hpp"


std::vector<sf::Sprite*> PassToPlayerScreenState::sprites;
std::vector<Button*> PassToPlayerScreenState::buttons;

PassToPlayerScreenState::PassToPlayerScreenState(StateManager& stateManager, sf::RenderWindow& window,
 const bool passToPlayerOne, const bool fleetDeployment, int FD_player1ShipLocations[12][12], bool FD_isPlayerTwo,
 int G_playerGridOriginal[12][12], int G_enemyGridOriginal[12][12],
 int G_playerGridCurrent[12][12], int G_enemyGridCurrent[12][12],
 int G_playerHitAttemptMap[12][12], int G_enemyHitAttemptMap[12][12],
 int G_playerHitMissMap[12][12], int G_enemyHitMissMap[12][12],
 bool G_enemyBlueSank, bool G_enemyGreenSank, bool G_enemyPinkSank, 
 bool G_enemyOrangeSank, bool G_enemyYellowSank,
 bool G_playerBlueSank, bool G_playerGreenSank, bool G_playerPinkSank, 
 bool G_playerOrangeSank, bool G_playerYellowSank,
 bool G_isPlayerOne)
 : State( stateManager, window ), fleetDeployment(fleetDeployment) {

    // Initialize transition values
    copy2dArray(FD_player1ShipLocations, this->FD_player1ShipLocations);
    this->FD_isPlayerTwo = FD_isPlayerTwo;
    copy2dArray(G_playerGridOriginal, this->G_playerGridOriginal);
    copy2dArray(G_enemyGridOriginal, this->G_enemyGridOriginal);
    copy2dArray(G_playerGridCurrent, this->G_playerGridCurrent);
    copy2dArray(G_enemyGridCurrent, this->G_enemyGridCurrent);
    copy2dArray(G_playerHitAttemptMap, this->G_playerHitAttemptMap);
    copy2dArray(G_enemyHitAttemptMap, this->G_enemyHitAttemptMap);
    copy2dArray(G_playerHitMissMap, this->G_playerHitMissMap);
    copy2dArray(G_enemyHitMissMap, this->G_enemyHitMissMap);
    this->G_enemyBlueSank = G_enemyBlueSank;
    this->G_enemyGreenSank = G_enemyGreenSank;
    this->G_enemyPinkSank = G_enemyPinkSank;
    this->G_enemyOrangeSank = G_enemyOrangeSank; 
    this->G_enemyYellowSank = G_enemyYellowSank;
    this->G_playerBlueSank = G_playerBlueSank;
    this->G_playerGreenSank = G_playerGreenSank;
    this->G_playerPinkSank = G_playerPinkSank;
    this->G_playerOrangeSank = G_playerOrangeSank;
    this->G_playerYellowSank = G_playerYellowSank;
    this->G_isPlayerOne = G_isPlayerOne;

    // Initialize sprites if not already initialized
    if (PassToPlayerScreenState::sprites.empty()) {
        PassToPlayerScreenState::sprites.push_back(initializeSprite(
            passToPlayerOne 
                ? *ResourceManager::getTexture(m_texturePaths[m_textureNames::PassToP1BackgroundTexture])
                : *ResourceManager::getTexture(m_texturePaths[m_textureNames::PassToP2BackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    if (passToPlayerOne) {
        sprites[m_spriteNames::PassToPlayerBackground]->setTexture(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::PassToP1BackgroundTexture])
        );
    } else {
        sprites[m_spriteNames::PassToPlayerBackground]->setTexture(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::PassToP2BackgroundTexture])
        );
    }

    // Initialize buttons if not already initialized
    if (PassToPlayerScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(120*4, 100*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::ContinueButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::ContinueButtonActiveTexture]),
            m_window
        ));
    }

    std::cout << "PassToPlayerScreenState Initialized\n";
}

PassToPlayerScreenState::~PassToPlayerScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);
    std::cout << "PassToPlayerScreenState Destroyed\n";
}

void PassToPlayerScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Left
                 && PassToPlayerScreenState::buttons[m_buttonNames::ContinueButton]->getButtonState()) {
                    playSound("buttonSelect.wav");

                    if (fleetDeployment) {
                        std::unique_ptr<State> state(new PassAndPlayFleetDeploymentScreenState(
                            m_stateManager, m_window,
                            FD_player1ShipLocations, FD_isPlayerTwo
                        ));
                        m_stateManager.changeState(std::move(state));
                    } else {
                        std::unique_ptr<State> state(new PassAndPlayGameplayScreenState(
                            m_stateManager, m_window,
                            G_playerGridOriginal, G_enemyGridOriginal,
                            G_playerGridCurrent, G_enemyGridCurrent,
                            G_playerHitAttemptMap, G_enemyHitAttemptMap,
                            G_playerHitMissMap, G_enemyHitMissMap,
                            G_enemyBlueSank, G_enemyGreenSank, G_enemyPinkSank, 
                            G_enemyOrangeSank, G_enemyYellowSank,
                            G_playerBlueSank, G_playerGreenSank, G_playerPinkSank, 
                            G_playerOrangeSank, G_playerYellowSank,
                            G_isPlayerOne
                        ));
                        m_stateManager.changeState(std::move(state));
                    }
                    
                    return;
                }
                return;
            }
            default:
                break;
        }
    }
}

void PassToPlayerScreenState::update() {
    sf::Vector2f mousePosition = PassToPlayerScreenState::getMousePosition();
    buttons[m_buttonNames::ContinueButton]->updateButtonState(mousePosition);
}

void PassToPlayerScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : PassToPlayerScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : PassToPlayerScreenState::buttons) {
        button->render();
    }

    m_window.display();
}
