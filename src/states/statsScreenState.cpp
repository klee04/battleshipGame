#include "statsScreenState.hpp"
#include "leaderboardScreenState.hpp"

#include <iostream>
#include <cmath>

std::vector<sf::Sprite*> StatsScreenState::sprites;
std::vector<Button*> StatsScreenState::buttons;

namespace stats {
    float round(float var) {
        float value = (int)(var * 100 + .5);
        return (float)value / 100;
    }
}

StatsScreenState::StatsScreenState(StateManager& stateManager, sf::RenderWindow& window)
: State( stateManager, window ) {

    // Initialize sprites if not already initialized
    if (StatsScreenState::sprites.empty()) {
        StatsScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::StatsBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (StatsScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(0, 0),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::BackButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::BackButtonActiveTexture]),
            m_window
        ));
    }

    // Initialize text
    m_totalWinsText = initializeText(
        "TOTAL WINS: " + std::to_string(m_stateManager.m_user->getTotalWins()),
        sf::Vector2f(80*4, 40*4),
        30*4,
        sf::Color::White
    );
    m_totalLossesText = initializeText(
        "TOTAL LOSSES: " + std::to_string(m_stateManager.m_user->getTotalLosses()),
        sf::Vector2f(80*4, 60*4),
        30*4,
        sf::Color::White
    );
    m_totalGamesText = initializeText(
        "TOTAL GAMES: " + std::to_string(m_stateManager.m_user->getTotalGames()),
        sf::Vector2f(80*4, 80*4),
        30*4,
        sf::Color::White
    );
    std::cout << 1.0 * m_stateManager.m_user->getTotalWins() << std::endl;
    m_winPercentageText = initializeText(
        "WIN PERCENTAGE: " + std::to_string((int)(m_stateManager.m_user->getWinPercentage() * 100.0)) + "%",
        sf::Vector2f(80*4, 100*4),
        30*4,
        sf::Color::White
    );

    std::cout << "StatsScreenState Initialized\n";
}

StatsScreenState::~StatsScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);

    // Delete text
    delete m_totalWinsText;
    m_totalWinsText = nullptr;
    delete m_totalLossesText;
    m_totalLossesText = nullptr;
    delete m_totalGamesText;
    m_totalGamesText = nullptr;
    delete m_winPercentageText;
    m_winPercentageText = nullptr;

    std::cout << "StatsScreenState Destroyed\n";
}

void StatsScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Left
                 && StatsScreenState::buttons[m_buttonNames::BackButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> leaderboardScreenState(new LeaderboardScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(leaderboardScreenState));
                    return;
                }
                return;
            }
            default:
                break;
        }
    }
}

void StatsScreenState::update() {
    sf::Vector2f mousePosition = StatsScreenState::getMousePosition();
    buttons[m_buttonNames::BackButton]->updateButtonState(mousePosition);
}

void StatsScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : StatsScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : StatsScreenState::buttons) {
        button->render();
    }
    
    m_window.draw(*m_totalWinsText);
    m_window.draw(*m_totalLossesText);
    m_window.draw(*m_totalGamesText);
    m_window.draw(*m_winPercentageText);

    m_window.display();
}
