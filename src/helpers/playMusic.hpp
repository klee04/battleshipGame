#pragma once
#include <string>
#include <SFML/Audio/Music.hpp>

inline static void playMusic(std::string filePath) {
    filePath = "./res/sounds/" + filePath;
    sf::Music* music = new sf::Music;
    music->openFromFile(filePath);
    music->setLoop(true);
    music->setVolume(60);
    music->play();
}
