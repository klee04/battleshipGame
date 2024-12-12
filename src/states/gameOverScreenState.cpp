#include "gameOverScreenState.hpp"
#include "menuScreenState.hpp"


std::vector<sf::Sprite*> GameOverScreenState::sprites;
std::vector<Button*> GameOverScreenState::buttons;

GameOverScreenState::GameOverScreenState(StateManager& stateManager, sf::RenderWindow& window,
 int gameResultIndex) : State( stateManager, window ) {

    // Initialize sprites if not already initialized
    if (GameOverScreenState::sprites.empty()) {
        GameOverScreenState::sprites.push_back(initializeSprite(
            *ResourceManager::getTexture(m_texturePaths[gameResultIndex]),
            sf::Vector2f(0,0),
            sf::Vector2f(4,4)
        ));
    }

    // Change texture
    sprites[m_spriteNames::Background]->setTexture(
        *ResourceManager::getTexture(m_texturePaths[gameResultIndex])
    );

    // Initialize buttons if not already initialized
    if (GameOverScreenState::buttons.empty()) {
        buttons.push_back(initializeButton(
            sf::Vector2f(120*4, 100*4),
            sf::Vector2f(4, 4),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::MenuButtonIdleTexture]),
            *ResourceManager::getTexture(m_texturePaths[m_textureNames::MenuButtonActiveTexture]),
            m_window
        ));
    }

    std::cout << "GameOverScreenState Initialized\n";
}

GameOverScreenState::~GameOverScreenState() {
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(cursor);
    std::cout << "GameOverScreenState Destroyed\n";
}

void GameOverScreenState::processEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_stateManager.quit();
                break;
            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Left
                 && GameOverScreenState::buttons[m_buttonNames::MenuButton]->getButtonState()) {
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

void GameOverScreenState::update() {
    sf::Vector2f mousePosition = GameOverScreenState::getMousePosition();
    buttons[m_buttonNames::MenuButton]->updateButtonState(mousePosition);
}

void GameOverScreenState::draw() {
    m_window.clear();

    for (sf::Sprite* sprite : GameOverScreenState::sprites) {
        m_window.draw(*sprite);
    }
    for (Button* button : GameOverScreenState::buttons) {
        button->render();
    }

    m_window.display();
}
