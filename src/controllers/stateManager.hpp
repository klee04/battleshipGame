#pragma once
#include "state.hpp"
#include "../database/database.h"
#include "../entity/user.hpp"
#include <stack>
#include <memory>

class StateManager {
    public:
        StateManager();
        ~StateManager();

        void pushState(std::unique_ptr<State> state);
        void popState();
        void changeState(std::unique_ptr<State> state);

        void processEvents();
        void update();
        void draw();

        std::unique_ptr<State> getCurrentState() { return std::move(m_states.top()); }

        bool running() { return m_running; }
        void quit() { m_running = false; }

        Database* m_database;
        User* m_user = nullptr;
        User* m_secondUser = nullptr;

    private:
        std::stack<std::unique_ptr<State>> m_states;
        bool m_running;
};
