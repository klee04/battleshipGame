#include "difficultySelectScreenState.hpp"
#include "menuScreenState.hpp"
#include "fleetDeploymentScreenState.hpp"

#include <iostream>

std::vector<sf::Sprite*> DifficultySelectScreenState::sprites;
std::vector<Button*> DifficultySelectScreenState::buttons;

DifficultySelectScreenState::DifficultySelectScreenState(StateManager& stateManager, sf::RenderWindow& window)
: State( stateManager, window ) {

    // Initialize sprites if not already initialized
    if (DifficultySelectScreenState::sprites.empty()) {
        DifficultySelectScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::DifficultySelectBackgroundTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (DifficultySelectScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(120*4, 60*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::EasyButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::EasyButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(120*4, 90*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::MediumButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::MediumButtonActiveTexture]),
            m_window
        ));

        buttons.push_back(initializeButton(
            sf::Vector2f(120*4, 120*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::HardButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::HardButtonActiveTexture]),
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

    std::cout << "DifficultySelectScreenState Initialized\n";
}

DifficultySelectScreenState::~DifficultySelectScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);
    std::cout << "DifficultySelectScreenState Destroyed\n";
}

void DifficultySelectScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Left
                 && DifficultySelectScreenState::buttons[m_buttonNames::EasyButton]->getButtonState()) {
                    // TODO: set bot difficulty to easy
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> fleetDeploymentScreenState(new FleetDeploymentScreenState(
                        m_stateManager, m_window, 0
                    ));
                    m_stateManager.changeState(std::move(fleetDeploymentScreenState));
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left
                 && DifficultySelectScreenState::buttons[m_buttonNames::MediumButton]->getButtonState()) {
                    // TODO: set bot difficulty to medium
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> fleetDeploymentScreenState(new FleetDeploymentScreenState(
                        m_stateManager, m_window, 1
                        ));
                    m_stateManager.changeState(std::move(fleetDeploymentScreenState));
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left
                 && DifficultySelectScreenState::buttons[m_buttonNames::HardButton]->getButtonState()) {
                    // TODO: set bot difficulty to hard
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> fleetDeploymentScreenState(new FleetDeploymentScreenState(
                        m_stateManager, m_window, 2
                    ));
                    m_stateManager.changeState(std::move(fleetDeploymentScreenState));
                    return;
                }

                if (event.mouseButton.button == sf::Mouse::Left
                 && DifficultySelectScreenState::buttons[m_buttonNames::BackButton]->getButtonState()) {
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

void DifficultySelectScreenState::update() {
    sf::Vector2f mousePosition = DifficultySelectScreenState::getMousePosition();
    buttons[m_buttonNames::EasyButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::MediumButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::HardButton]->updateButtonState(mousePosition);
    buttons[m_buttonNames::BackButton]->updateButtonState(mousePosition);
}

void DifficultySelectScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : DifficultySelectScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : DifficultySelectScreenState::buttons) {
        button->render();
    }

    m_window.display();
}
