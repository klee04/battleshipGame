#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class ResourceManager {
    private:

        const static std::vector<std::string> TEXTURE_EXTENSIONS;
        const static std::vector<std::string> SOUND_EXTENSIONS;
        const static std::vector<std::string> FONT_EXTENSIONS;

        static std::map<std::string, sf::Texture*> m_textureMap;
        static std::map<std::string, sf::SoundBuffer*> m_soundMap;
        static std::map<std::string, sf::Font*> m_fontMap;

    public:

        static sf::Texture* getTexture(const std::string filePath);
        static void preLoadTextures(const std::string folderPath);
        static void clearTextures();

        static sf::SoundBuffer* getSoundBuffer(const std::string filePath);
        static void preLoadSoundBuffers(const std::string folderPath);
        static void clearSoundBuffers();

        static sf::Font* getFont(const std::string filePath);
        static void preLoadFonts(const std::string folderPath);
        static void clearFonts();

        static bool contains(std::vector<std::string> vec, std::string str);
};
