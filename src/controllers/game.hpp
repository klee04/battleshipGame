#pragma once
#include "../states/titleScreenState.hpp"
#include "../helpers/playMusic.hpp"

class Game {
    public:
        Game() {};
        
        void run();
    private:
        static const sf::Time TimePerFrame;
        
        StateManager m_stateManager;
        sf::RenderWindow m_window;

        constexpr static int m_width = 1280;
        constexpr static int m_height = 720;
};
