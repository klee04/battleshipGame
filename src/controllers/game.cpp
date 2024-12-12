#include "game.hpp"

const sf::Time Game::TimePerFrame = sf::seconds(1.0f / 30.0f);

void Game::run() {
    m_window.create(sf::VideoMode{ m_width, m_height }, "Battleship", sf::Style::Titlebar | sf::Style::Close);
    m_window.setMouseCursorVisible(true);
    m_window.setKeyRepeatEnabled(false);

    // Preload all resources
    ResourceManager::preLoadTextures("./res/images");
    ResourceManager::preLoadSoundBuffers("./res/sounds");
    ResourceManager::preLoadFonts("./res/fonts");

    // Play background music
    playMusic("backgroundMusic.mp3");

    // Move to title screen
    std::unique_ptr<State> titleScreenState(new TitleScreenState(m_stateManager, m_window));
    m_stateManager.changeState(std::move(titleScreenState));

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    // Game loop
    while (m_stateManager.running()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;

            m_stateManager.processEvents();
            m_stateManager.update();
        }
        m_stateManager.draw();
    }
}