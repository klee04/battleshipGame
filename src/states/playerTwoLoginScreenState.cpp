#include "playerTwoLoginScreenState.hpp"
#include "passAndPlayFleetDeploymentScreenState.hpp"
#include "menuScreenState.hpp"
#include "modifiedLoginScreenState.hpp"

#include <iostream>

std::vector<sf::Sprite*> PlayerTwoLoginScreenState::sprites;
std::vector<Button*> PlayerTwoLoginScreenState::buttons;

PlayerTwoLoginScreenState::PlayerTwoLoginScreenState(StateManager& stateManager, sf::RenderWindow& window)
: State( stateManager, window ) {

    // Initialize sprites if not already initialized
    if (PlayerTwoLoginScreenState::sprites.empty()) {
        PlayerTwoLoginScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::PlayerTwoLoginBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (PlayerTwoLoginScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(60*4, 100*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LoginButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::LoginButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(180*4, 100*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::GuestButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::GuestButtonActiveTexture]),
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

    std::cout << "PlayerTwoLoginScreenState Initialized\n";
}

PlayerTwoLoginScreenState::~PlayerTwoLoginScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);
    std::cout << "PlayerTwoLoginScreenState Destroyed\n";
}

void PlayerTwoLoginScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {

                // If left click on login button
                if (event.mouseButton.button == sf::Mouse::Left
                 && PlayerTwoLoginScreenState::buttons[m_buttonNames::LoginButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> modifiedLoginScreenState(new ModifiedLoginScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(modifiedLoginScreenState));
                    return;
                }

                // If left click on guest button
                if (event.mouseButton.button == sf::Mouse::Left
                 && PlayerTwoLoginScreenState::buttons[m_buttonNames::GuestButton]->getButtonState()) {

                    // Login Second Player
                    m_stateManager.m_secondUser = new User(m_stateManager.m_database);

                    // Proceed to fleet deployment
                    int emptyArray[12][12] = {};
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> passAndPlayFleetDeploymentScreenState(new PassAndPlayFleetDeploymentScreenState(
                        m_stateManager, m_window, emptyArray, false
                    ));
                    m_stateManager.changeState(std::move(passAndPlayFleetDeploymentScreenState));
                    return;
                }

                // If left click on back button
                if (event.mouseButton.button == sf::Mouse::Left
                 && PlayerTwoLoginScreenState::buttons[m_buttonNames::BackButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
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

void PlayerTwoLoginScreenState::update() {
    sf::Vector2f mousePosition = PlayerTwoLoginScreenState::getMousePosition();
    buttons[m_buttonNames::LoginButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::GuestButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::BackButton]->updateButtonState(mousePosition);
}

void PlayerTwoLoginScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : PlayerTwoLoginScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : PlayerTwoLoginScreenState::buttons) {
        button->render();
    }

    m_window.display();
}
