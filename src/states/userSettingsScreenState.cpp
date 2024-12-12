#include "userSettingsScreenState.hpp"
#include "../helpers/initializeUserIcon.hpp"
#include "menuScreenState.hpp"
#include "loginScreenState.hpp"

#include <iostream>

std::vector<sf::Sprite*> UserSettingsScreenState::sprites;
std::vector<Button*> UserSettingsScreenState::buttons;

UserSettingsScreenState::UserSettingsScreenState(StateManager& stateManager, sf::RenderWindow& window)
: State( stateManager, window ), m_iconColourIndex(m_stateManager.m_user->getIconColour()) {
    
    m_usernameText = *initializeText(
        "User Settings",
        sf::Vector2f(87*4, 33*4), 
        40 * 4,  
        sf::Color::White
    );
    m_userIcon = *initializeUserIcon(
        sf::Vector2f(150*4, 80*4), 
        40, 
        m_iconColours[m_iconColourIndex] 
    );

    // Initialize sprites if not already initialized
    if (UserSettingsScreenState::sprites.empty()) {
        UserSettingsScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::UserSettingsBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (UserSettingsScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(120*4, 80*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LeftButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LeftButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(180*4, 80*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::RightButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::RightButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(120*4, 120*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LogoutButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LogoutButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(0, 0),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::BackButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::BackButtonActiveTexture]),
            m_window
        ));
    }

    std::cout << "UserSettingsScreenState Initialized\n";
}

UserSettingsScreenState::~UserSettingsScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);
    std::cout << "UserSettingsScreenState Destroyed\n";
}

sf::Color UserSettingsScreenState::getNextColour() {
    if (m_iconColourIndex == 5) {
        m_iconColourIndex = 0;
    } else {
        m_iconColourIndex++;
    }

    return m_iconColours[m_iconColourIndex];
}

sf::Color UserSettingsScreenState::getPreviousColour() {
    if (m_iconColourIndex == 0) {
        m_iconColourIndex = 5;
    } else {
        m_iconColourIndex--;
    }

    return m_iconColours[m_iconColourIndex];
}

void UserSettingsScreenState::changeIconColour(sf::CircleShape& userIcon, bool rightButton) {
    if (rightButton) {
        userIcon.setFillColor(getNextColour());
    } else {
        userIcon.setFillColor(getPreviousColour());
    }

    // Load icon colour to database
    m_stateManager.m_user->updateUserIcon(m_iconColourIndex);
}

void UserSettingsScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;

            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Left && buttons[m_buttonNames::BackButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> menuScreenState(new MenuScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(menuScreenState));
                }
                if (event.mouseButton.button == sf::Mouse::Left && buttons[m_buttonNames::LogoutButton]->getButtonState()) {
                    playSound("logoutButtonSelect.wav");

                    // Destroy User object when the user logs out
                    delete m_stateManager.m_user;
                    m_stateManager.m_user = nullptr;

                    std::unique_ptr<State> loginScreenState(new LoginScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(loginScreenState));
                }
                if (event.mouseButton.button == sf::Mouse::Left && buttons[m_buttonNames::RightButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    changeIconColour(m_userIcon, true);
                }
                if (event.mouseButton.button == sf::Mouse::Left && buttons[m_buttonNames::LeftButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    changeIconColour(m_userIcon, false);
                }
                return;
            }

            default:
                break;
        }
    }
}

void UserSettingsScreenState::update() {
    sf::Vector2f mousePosition = UserSettingsScreenState::getMousePosition();
    buttons[m_buttonNames::LeftButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::RightButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::LogoutButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::BackButton]->updateButtonState(mousePosition);
}

void UserSettingsScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : UserSettingsScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : UserSettingsScreenState::buttons) {
        button->render();
    }

    m_window.draw(m_usernameText);
    m_window.draw(m_userIcon);

    m_window.display();
}
