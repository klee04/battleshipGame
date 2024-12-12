#pragma once
#include <utility>
#include <string>
#include <vector>
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "../controllers/resourceManager.hpp"

class GridCell {
    public:
        GridCell();
        GridCell(const int& firstGridPosition, const int& secondGridPosition, 
                 const sf::Vector2f& globalPosition, const bool spriteNeeded = false);
        ~GridCell();

        bool isInsideRect(const sf::Vector2f& position);

        sf::Vector2f getPosition();
        void setSpriteTexture(const int& textureIndex);
        void updateHitMarker(const sf::Vector2f& mousePosition, const bool& hitAttemptMade = false);

        void render(sf::RenderWindow& window) const;

    private:
        std::pair<int, int> m_gridPosition;

        sf::FloatRect* m_rect;
        sf::Sprite* m_sprite;
        sf::Sprite* m_hitMarker;

        bool spritePlaced = false;
        bool showHitMarker = false;

        std::vector<std::string> m_texturePaths{
            "GameplayScreen/hit.png",
            "GameplayScreen/miss.png",
            "GameplayScreen/hitMarker.png"
        };

        enum m_textureNames {
            HitTexture,
            MissTexture,
            HitMarker
        };
        

};
