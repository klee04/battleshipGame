#pragma once
#include "resourceManager.hpp"
#include "../helpers/initializeSprite.hpp"
#include "../helpers/initializeButton.hpp"
#include "../helpers/initializeText.hpp"
#include "../helpers/playSound.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

class StateManager;

class State {
    public:
        State(StateManager& stateManager, sf::RenderWindow& window)
        : m_stateManager{ stateManager }
        , m_window{ window } {}

        virtual ~State() {}

        virtual void processEvents() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;

    protected:
        StateManager& m_stateManager;
        sf::RenderWindow& m_window;

        sf::Vector2f getMousePosition() { return m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)); };
};
