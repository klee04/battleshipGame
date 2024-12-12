#include "resourceManager.hpp"

const std::vector<std::string> ResourceManager::TEXTURE_EXTENSIONS = {"png", "jpg", "jpeg"};
const std::vector<std::string> ResourceManager::SOUND_EXTENSIONS = {"wav"};
const std::vector<std::string> ResourceManager::FONT_EXTENSIONS = {"ttf", "mp3"};

std::map<std::string, sf::Texture*> ResourceManager::m_textureMap;
std::map<std::string, sf::SoundBuffer*> ResourceManager::m_soundMap;
std::map<std::string, sf::Font*> ResourceManager::m_fontMap;

sf::Texture* ResourceManager::getTexture(std::string filePath) {
  filePath = "./res/images/" + filePath;

  // Search through the map to see if there is already an entry
  if (m_textureMap.find(filePath) != m_textureMap.end()) {
      return m_textureMap[filePath];
  }

  // If the code has made it to this point, it hasn't found a matching entry
  // in the map. We use the new keyword because we want to store these variables
  // outside of the stack
  sf::Texture* texture = new sf::Texture();
  
  texture->loadFromFile(filePath);

  m_textureMap[filePath] = texture;

  return m_textureMap[filePath];
}

void ResourceManager::preLoadTextures(const std::string folderPath) {
  for (auto& file: std::filesystem::recursive_directory_iterator(folderPath)) {
    // Use a stringstream to get the path as a string here
    std::stringstream ss;
    ss << file;

    // Check the extension
    if (contains(TEXTURE_EXTENSIONS, ss.str().substr(ss.str().length() - 4, 3))) {
      sf::Texture* texture = new sf::Texture();
      texture->loadFromFile(ss.str().substr(1, ss.str().length() - 2));
      m_textureMap[ss.str().substr(1, ss.str().length() - 2)] = texture;
    }
  }

  std::cout << "Textures Loaded" << "\n";
}

void ResourceManager::clearTextures() {
  // First delete all of the pointers
  for (auto element: m_textureMap) {
    delete element.second;
  }

  // And now clear all of the entries
  m_textureMap.clear();
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(std::string filePath) {
  filePath = "./res/sounds/" + filePath;

  // Search through the map to see if there is already an entry
  if (m_soundMap.find(filePath) != m_soundMap.end()) {
      return m_soundMap[filePath];
  }

  // If the code has made it to this point, it hasn't found a matching entry
  // in the map. We use the new keyword because we want to store these variables
  // outside of the stack
  sf::SoundBuffer* sound = new sf::SoundBuffer();

  sound->loadFromFile(filePath);

  m_soundMap[filePath] = sound;

  return m_soundMap[filePath];
}

void ResourceManager::preLoadSoundBuffers(const std::string folderPath) {
  for (auto& file: std::filesystem::recursive_directory_iterator(folderPath)) {
    std::stringstream ss;
    ss << file;

    if (contains(SOUND_EXTENSIONS, ss.str().substr(ss.str().length() - 4, 3))) {
      sf::SoundBuffer* sound = new sf::SoundBuffer();
      sound->loadFromFile(ss.str().substr(1, ss.str().length() - 2));
      m_soundMap[ss.str().substr(1, ss.str().length() - 2)] = sound;
    }
  }

  std::cout << "Sounds Loaded\n";
}

void ResourceManager::clearSoundBuffers() {
  // Delete all of pointers
  for (auto element: m_soundMap) {
    delete element.second;
  }

  // Clear all of entries
  m_soundMap.clear();
}

sf::Font* ResourceManager::getFont(std::string filePath) {
  filePath = "./res/fonts/" + filePath;

  // Search through the map to see if there is already an entry
  if (m_fontMap.find(filePath) != m_fontMap.end()) {
      return m_fontMap[filePath];
  }

  // If the code has made it to this point, it hasn't found a matching entry
  // in the map. We use the new keyword because we want to store these variables
  // outside of the stack
  sf::Font* font = new sf::Font();

  font->loadFromFile(filePath);

  m_fontMap[filePath] = font;

  return m_fontMap[filePath];
}

void ResourceManager::preLoadFonts(const std::string folderPath) {
  for (auto& file: std::filesystem::recursive_directory_iterator(folderPath)) {
    std::stringstream ss;
    ss << file;

    if (contains(FONT_EXTENSIONS, ss.str().substr(ss.str().length() - 4, 3))) {
      sf::Font* font = new sf::Font();
      font->loadFromFile(ss.str().substr(1, ss.str().length() - 2));
      m_fontMap[ss.str().substr(1, ss.str().length() - 2)] = font;
    }
  }

  std::cout << "Fonts Loaded\n";
}

void ResourceManager::clearFonts() {
  // Delete all of pointers
  for (auto element: m_fontMap) {
    delete element.second;
  }

  // Clear all of entries
  m_fontMap.clear();
}

bool ResourceManager::contains(std::vector<std::string> vec, std::string str) {
  for (std::string s: vec) {
    if (s == str)
      return true;
  }
  return false;
}
