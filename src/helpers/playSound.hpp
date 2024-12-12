#pragma once
#include "../controllers/resourceManager.hpp"

inline static void playSound(std::string filePath) {
    sf::Sound* sound = new sf::Sound(*ResourceManager::getSoundBuffer(filePath));
    sound->play();
}
