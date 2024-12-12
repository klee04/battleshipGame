#pragma once
#include <utility>
#include <vector>
#include "SFML/Graphics/Sprite.hpp"
#include "../controllers/resourceManager.hpp"

class MiniGridCell {
    public:
        MiniGridCell();
        MiniGridCell(const int& firstGridPosition, const int& secondGridPosition, 
                     const sf::Vector2f& globalPosition);
        ~MiniGridCell();

        void setBottomTexture(const int& textureIndex);
        void setTopTexture(const int& textureIndex);

        void render(sf::RenderWindow& window) const;

    private:
        std::pair<int, int> m_gridPosition;

        sf::Sprite* m_bottomSprite;  // for ship
        sf::Sprite* m_topSprite;  // for hit/miss indicator

        std::vector<std::string> m_texturePaths{
            "GameplayScreen/miniHit.png",
            "GameplayScreen/miniMiss.png",
            "GameplayScreen/miniShipBlue.png",
            "GameplayScreen/miniShipGreen.png",
            "GameplayScreen/miniShipPink.png",
            "GameplayScreen/miniShipOrange.png",
            "GameplayScreen/miniShipYellow.png"
        };

        enum m_textureNames {
            MiniHitTexture,
            MiniMissTexture,
            MiniShipBlueTexture,
            MiniShipGreenTexture,
            MiniShipPinkTexture,
            MiniShipOrangeTexture,
            MiniShipYellowTexture
        };
        

};