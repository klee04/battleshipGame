#include "titleScreenState.hpp"
#include "loginScreenState.hpp"

#include <iostream>

std::vector<sf::Sprite*> TitleScreenState::sprites;
std::vector<Button*> TitleScreenState::buttons;

TitleScreenState::TitleScreenState(StateManager& stateManager, sf::RenderWindow& window)
: State( stateManager, window ) {

    animationSwitch = false;

    // Initialize sprites if not already initialized
    if (TitleScreenState::sprites.empty()) {
        TitleScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::TitleScreenTexture]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));

        TitleScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::TitleTextTexture]),
            sf::Vector2f(31*4, 53*4),
            sf::Vector2f(4,4)
        ));
    }

    // Initialize buttons if not already initialized
    if (TitleScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(210*4, 110*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::PlayButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::PlayButtonActiveTexture]),
            m_window
        ));
    }

    std::cout << "TitleScreenState Initialized\n";
}

TitleScreenState::~TitleScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);
    std::cout << "TitleScreenState Destroyed\n";
}

void TitleScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Left && TitleScreenState::buttons[m_buttonNames::PlayButton]->getButtonState()) {
                    playSound("buttonSelect.wav");
                    std::unique_ptr<State> loginScreenState(new LoginScreenState(m_stateManager, m_window));
                    m_stateManager.changeState(std::move(loginScreenState));
                }
                return;
            }
            default:
                break;
        }
    }
}

void TitleScreenState::update() {
    sf::Vector2f mousePosition = TitleScreenState::getMousePosition();
    buttons[m_buttonNames::PlayButton]->updateButtonState(mousePosition);


    // Text floating animation
    float globalTop = sprites[m_spriteNames::TitleText]->getGlobalBounds().top;
    if (globalTop <= 202) animationSwitch = true;
    else if (globalTop >= 212) animationSwitch = false;
    
    if (animationSwitch) 
        sprites[m_spriteNames::TitleText]->move(sf::Vector2f(0, 0.5));
    else 
        sprites[m_spriteNames::TitleText]->move(sf::Vector2f(0, -0.5));
}

void TitleScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : TitleScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : TitleScreenState::buttons) {
        button->render();
    }

    m_window.display();
}
