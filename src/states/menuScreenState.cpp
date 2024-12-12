#include "menuScreenState.hpp"
#include "../helpers/initializeUserIcon.hpp"
#include "userSettingsScreenState.hpp"
#include "difficultySelectScreenState.hpp"
#include "playerTwoLoginScreenState.hpp"
#include "leaderboardScreenState.hpp"
#include "loginScreenState.hpp"


#include <iostream>

std::vector<sf::Sprite*> MenuScreenState::sprites;
std::vector<Button*> MenuScreenState::buttons;

MenuScreenState::MenuScreenState(StateManager& stateManager, sf::RenderWindow& window)
: State( stateManager, window ) {
    
    // Initialize username text
    m_usernameText = initializeText(
        m_stateManager.m_user->getUsername(),
        m_stateManager.m_user->getIsGuest() ? sf::Vector2f(85*4, 4*4) : sf::Vector2f(25*4, 4*4), 
        20 * 4, 
        sf::Color::White
    );

    // Initialize user icon colour
    m_userIcon = initializeUserIcon(
        sf::Vector2f(m_usernameText->getGlobalBounds().left, 25) + sf::Vector2f(m_usernameText->getGlobalBounds().width + 15, 0), 
        15, 
        m_iconColours[m_stateManager.m_user->getIconColour()]
    );

    // Initialize sprites if not already initialized
    if (MenuScreenState::sprites.empty()) {
        MenuScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::MenuBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (MenuScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(0, 40*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::PlayVsBotButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::PlayVsBotButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(110*4, 40*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::PassAndPlayButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::PassAndPlayButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(220*4, 40*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LeaderboardButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LeaderboardButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(0, 0),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::UserSettingsButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::UserSettingsButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(0, 0),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LogoutButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LogoutButtonActiveTexture]),
            m_window
        ));
    }

    std::cout << "MenuScreenState Initialized\n";
}

MenuScreenState::~MenuScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);

    delete m_usernameText;
    m_usernameText = nullptr;
    delete m_userIcon;
    m_userIcon = nullptr;

    std::cout << "MenuScreenState Destroyed\n";
}

void MenuScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;

            case sf::Event::MouseButtonReleased: {

                if (event.mouseButton.button == sf::Mouse::Left 
                 && MenuScreenState::buttons[m_buttonNames::UserSettingsButton]->getButtonState()
                 && !m_stateManager.m_user->getIsGuest()) {

                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> userSettingsScreenState(new UserSettingsScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(userSettingsScreenState));
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left 
                 && MenuScreenState::buttons[m_buttonNames::LogoutButton]->getButtonState()
                 && m_stateManager.m_user->getIsGuest()) {

                    playSound("logoutButtonSelect.wav");
                    std::unique_ptr<State> loginScreenState(new LoginScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(loginScreenState));
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left 
                 && MenuScreenState::buttons[m_buttonNames::PlayVsBotButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> difficultySelectScreenState(new DifficultySelectScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(difficultySelectScreenState));
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left 
                 && MenuScreenState::buttons[m_buttonNames::PassAndPlayButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> playerTwoLoginScreenState(new PlayerTwoLoginScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(playerTwoLoginScreenState));
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left 
                 && MenuScreenState::buttons[m_buttonNames::LeaderboardButton]->getButtonState()) {
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

void MenuScreenState::update() {
    sf::Vector2f mousePosition = MenuScreenState::getMousePosition();
    buttons[m_buttonNames::PlayVsBotButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::PassAndPlayButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::LeaderboardButton]->updateButtonState(mousePosition);
    if (m_stateManager.m_user->getIsGuest()) 
        buttons[m_buttonNames::LogoutButton]->updateButtonState(mousePosition);
    else
        buttons[m_buttonNames::UserSettingsButton]->updateButtonState(mousePosition);
}

void MenuScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : MenuScreenState::sprites) {
        m_window.draw(*sprite);
    }
    // for (Button* button : MenuScreenState::buttons) {
    //     button->render();
    // }

    for (int i = 0; i < buttons.size(); i++) {
        if (i == 3 && m_stateManager.m_user->getIsGuest()) continue;  // Don't draw user settings button if guest
        if (i == 4 && !m_stateManager.m_user->getIsGuest()) continue;  // Don't draw logout button if not guest
        buttons[i]->render();
    }

    m_window.draw(*m_usernameText);
    m_window.draw(*m_userIcon);

    m_window.display();
}
