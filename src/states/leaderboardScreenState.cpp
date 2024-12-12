/*
ONLY WORKS WHEN THERE'S AT LEAST 5 USERS IN DATABASE TXT FILE
*/

#include "leaderboardScreenState.hpp"
#include "menuScreenState.hpp"
#include "statsScreenState.hpp"

#include <iostream>

std::vector<sf::Sprite*> LeaderboardScreenState::sprites;
std::vector<Button*> LeaderboardScreenState::buttons;

enum stats {
    TotalWins,
    TotalGamesPlayed
};

LeaderboardScreenState::LeaderboardScreenState(StateManager& stateManager, sf::RenderWindow& window)
: State( stateManager, window ) {

    // Initialize sprites if not already initialized
    if (LeaderboardScreenState::sprites.empty()) {
        LeaderboardScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LeaderboardBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (LeaderboardScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(0, 0),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::BackButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::BackButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(220*4, 150*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::YourStatsButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::YourStatsButtonActiveTexture]),
            m_window
        ));
    }

    // Initialize filter button
    m_filterButtonRect = new sf::Rect<float>(
        sf::Vector2f(90*4, 150*4),
        sf::Vector2f(90*4, 20*4)
    );

    // Initialize filter text
    m_filterText = initializeText(
        m_filterTextOptions[m_currentStatIndex],
        sf::Vector2f(94*4, 153*4),
        24*4,
        sf::Color::Black
    );

    // Initialize user stat bars
    std::vector<std::unordered_map<std::string, std::string>> leaderboardMap =
     m_stateManager.m_database->getLeaderboardStats(
        stats::TotalWins,
        m_stateManager.m_user->getIsGuest(),
        !m_stateManager.m_user->getIsGuest() ? m_stateManager.m_user->getUsername() : ""
    );
    int startWidth = 30 * 4;
    int startHeight = 40 * 4;
    for (int i = 0; i < 5; i++) {
        m_userStatBars.push_back(new UserStatBar(
            sf::Vector2f(startWidth, startHeight + i * (20 * 4)),
            sf::Vector2f(4, 4),
            std::stoi(leaderboardMap[i]["rankingNumber"]),  // ranking number
            std::stoi(leaderboardMap[i]["iconColour"]),  // user icon colour
            leaderboardMap[i]["username"],  // username
            stats::TotalWins,  // stat to display
            std::stoi(leaderboardMap[i]["numberToDisplay"]),  // number to display
            m_window,
            i == 4 && !m_stateManager.m_user->getIsGuest() ? true : false  // isGuest
        ));
    }

    std::cout << "LeaderboardScreenState Initialized\n";
}

LeaderboardScreenState::~LeaderboardScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);

    delete m_filterButtonRect;
    m_filterButtonRect = nullptr;
    delete m_filterText;
    m_filterText = nullptr;

    // Delete user stat bars
    for (int i = 0; i < m_userStatBars.size(); i++) {
        delete m_userStatBars[i];
        m_userStatBars[i] = nullptr;
        m_userStatBars.pop_back();
    }

    std::cout << "LeaderboardScreenState Destroyed\n";
}

void LeaderboardScreenState::updateUserStatBars(int statIndex) {
    // Generate unordered_map for stats
    std::vector<std::unordered_map<std::string, std::string>> leaderboardMap =
     m_stateManager.m_database->getLeaderboardStats(
        statIndex,
        m_stateManager.m_user->getIsGuest(),
        !m_stateManager.m_user->getIsGuest() ? m_stateManager.m_user->getUsername() : ""
    );

    // Update stat bars
    for (int i = 0; i < leaderboardMap.size(); i++) {
        m_userStatBars[i]->updateInfo(
            std::stoi(leaderboardMap[i]["rankingNumber"]),
            std::stoi(leaderboardMap[i]["iconColour"]),
            leaderboardMap[i]["username"],
            statIndex,
            std::stoi(leaderboardMap[i]["numberToDisplay"])
        );
    }
}

void LeaderboardScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {
                
                if (event.mouseButton.button == sf::Mouse::Left
                 && LeaderboardScreenState::buttons[m_buttonNames::BackButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> menuScreenState(new MenuScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(menuScreenState));
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left
                 && LeaderboardScreenState::buttons[m_buttonNames::YourStatsButton]->getButtonState()) {
                    if (m_stateManager.m_user->getIsGuest()) {
                        playSound("invalidButtonSelect.wav");
                        return;
                    }
                    
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> statsScreenState(new StatsScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(statsScreenState));
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left && m_filterButtonHovered) {
                    playSound("buttonSelect.wav");
                    
                    // Update stat index
                    if (m_currentStatIndex == 1)
                        m_currentStatIndex = 0;
                    else {
                        m_currentStatIndex++;
                    }

                    // Update filter text
                    m_filterText->setString(m_filterTextOptions[m_currentStatIndex]);
                    
                    // Update stat bars
                    updateUserStatBars(m_currentStatIndex);

                    return;
                }

                return;
            }
            default:
                break;
        }
    }
}

void LeaderboardScreenState::update() {
    sf::Vector2f mousePosition = LeaderboardScreenState::getMousePosition();
    buttons[m_buttonNames::BackButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::YourStatsButton]->updateButtonState(mousePosition);

    if (m_filterButtonRect->contains(mousePosition) && !m_filterButtonHovered) 
        m_filterButtonHovered = true;
    if (!m_filterButtonRect->contains(mousePosition) && m_filterButtonHovered) 
        m_filterButtonHovered = false;
    
}

void LeaderboardScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : LeaderboardScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : LeaderboardScreenState::buttons) {
        button->render();
    }
    for (UserStatBar* userStatBar : LeaderboardScreenState::m_userStatBars) {
        userStatBar->render(m_window);
    }

    m_window.draw(*m_filterText);

    m_window.display();
}
